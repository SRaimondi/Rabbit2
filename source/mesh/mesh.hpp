//
// Created by Simon on 2019-03-26.
//

#ifndef RABBIT2_MESH_HPP
#define RABBIT2_MESH_HPP

#include "geometry/bbox.hpp"
#include "geometry/intersection.hpp"
#include "geometry/transform.hpp"
#include "material/material.hpp"

#include <vector>
#include <ostream>
#include <memory>

namespace Rabbit
{

class Triangle;

struct TriangleDescription
{
    constexpr TriangleDescription(unsigned int v0,
                                  unsigned int v1,
                                  unsigned int v2,
                                  unsigned int n0 = INVALID_INDEX,
                                  unsigned int n1 = INVALID_INDEX,
                                  unsigned int n2 = INVALID_INDEX,
                                  unsigned int uv0 = INVALID_INDEX,
                                  unsigned int uv1 = INVALID_INDEX,
                                  unsigned int uv2 = INVALID_INDEX)
        : v0{ v0 }, v1{ v1 }, v2{ v2 },
          n0{ n0 }, n1{ n1 }, n2{ n2 },
          uv0{ uv0 }, uv1{ uv1 }, uv2{ uv2 }
    {}

    static constexpr unsigned int INVALID_INDEX{ std::numeric_limits<unsigned int>::max() };

    // Vertices indices
    const unsigned int v0;
    const unsigned int v1;
    const unsigned int v2;
    // Normals indices
    const unsigned int n0;
    const unsigned int n1;
    const unsigned int n2;
    // UV indices
    const unsigned int uv0;
    const unsigned int uv1;
    const unsigned int uv2;
};

class Mesh
{
public:
    Mesh(std::vector<Geometry::Point3f>&& v, std::vector<Geometry::Vector3f>&& n,
         std::vector<Geometry::Vector2f>&& uvs, std::vector<TriangleDescription>&& tr);

    const Geometry::Point3f& VertexAt(unsigned int vertex_index) const noexcept
    {
        assert(vertex_index < vertices.size());
        return vertices[vertex_index];
    }

    bool HasNormals() const
    {
        return !normals.empty();
    }

    const Geometry::Vector3f& NormalAt(unsigned int normal_index) const noexcept
    {
        assert(normal_index < normals.size());
        return normals[normal_index];
    }

    bool HasUVs() const noexcept
    {
        return !uvs.empty();
    }

    const Geometry::Vector2f& UVAt(unsigned int uv_index) const noexcept
    {
        assert(uv_index < uvs.size());
        return uvs[uv_index];
    }

    // Create list of triangles for the mesh
    std::vector<Triangle> CreateTriangles(const std::shared_ptr<const Geometry::Transform>& transform,
                                          const std::shared_ptr<const MaterialInterface>& material) const noexcept;

    // Appen triangles to given list
    void CreateTriangles(const std::shared_ptr<const Geometry::Transform>& transform,
                         const std::shared_ptr<const MaterialInterface>& material,
                         std::vector<Triangle>& triangles_list) const noexcept;

private:
    // Mesh representation
    std::vector<Geometry::Point3f> vertices;
    std::vector<Geometry::Vector3f> normals;
    std::vector<Geometry::Vector2f> uvs;
    std::vector<TriangleDescription> triangles;
};

class Triangle
{
public:
    Triangle(const TriangleDescription& description, const Mesh& m,
             const std::shared_ptr<const Geometry::Transform>& transform,
             const std::shared_ptr<const MaterialInterface>& material) noexcept;

    // Compute triangle BBox in world space
    const Geometry::BBox Bounds() const noexcept
    {
        return { transformation->ToWorld(mesh.VertexAt(description.v0)),
                 transformation->ToWorld(mesh.VertexAt(description.v1)),
                 transformation->ToWorld(mesh.VertexAt(description.v2)) };
    }

    // Intersect ray with triangle
    void Intersect(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                   Geometry::TriangleIntersection& intersection) const noexcept;

    // Check for intersection
    bool IntersectTest(const Geometry::Ray& ray, const Geometry::Intervalf& interval) const noexcept;

    // Fill geometry information, ray is assumed to be in world space so we can compute the hit point directly
    void ComputeIntersectionGeometry(const Geometry::Ray& ray,
                                     float intersection_parameter,
                                     Geometry::TriangleIntersection& intersection) const noexcept;

    // Pointer to the material
    std::shared_ptr<const MaterialInterface> material;

private:
    // Reference to the triangle description
    const TriangleDescription& description;
    // Mesh associated
    const Mesh& mesh;
    // Transformation for the triangle
    std::shared_ptr<const Geometry::Transform> transformation;
};

inline void Triangle::Intersect(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                                Geometry::TriangleIntersection& intersection) const noexcept
{
    const Geometry::Point3f local_origin{ transformation->ToLocal(ray.Origin()) };
    const Geometry::Vector3f local_direction{ transformation->ToLocal(ray.Direction()) };

    // Translate vertices based on ray origin
    Geometry::Vector3f v0t{ mesh.VertexAt(description.v0) - local_origin };
    Geometry::Vector3f v1t{ mesh.VertexAt(description.v1) - local_origin };
    Geometry::Vector3f v2t{ mesh.VertexAt(description.v2) - local_origin };

    // Permute components of triangle vertices and ray direction
    const unsigned int kz{ Abs(local_direction).LargestDimension() };
    unsigned int kx = kz + 1;
    if (kx == 3)
    {
        kx = 0;
    }
    unsigned int ky = kx + 1;
    if (ky == 3)
    {
        ky = 0;
    }
    const Geometry::Vector3f d{ Permute(local_direction, kx, ky, kz) };
    v0t = Permute(v0t, kx, ky, kz);
    v1t = Permute(v1t, kx, ky, kz);
    v2t = Permute(v2t, kx, ky, kz);

    // Apply shear transformation to translated vertex positions
    const float sz{ 1.f / d.z };
    const float sx{ -d.x * sz };
    const float sy{ -d.y * sz };
    v0t.x += sx * v0t.z;
    v0t.y += sy * v0t.z;
    v1t.x += sx * v1t.z;
    v1t.y += sy * v1t.z;
    v2t.x += sx * v2t.z;
    v2t.y += sy * v2t.z;

    // Compute edge function coefficients e0, e1, and e2
    const float e0{ v1t.x * v2t.y - v1t.y * v2t.x };
    const float e1{ v2t.x * v0t.y - v2t.y * v0t.x };
    const float e2{ v0t.x * v1t.y - v0t.y * v1t.x };

    // Perform triangle edge and determinant tests
    if ((e0 < 0.f || e1 < 0.f || e2 < 0.f) && (e0 > 0.f || e1 > 0.f || e2 > 0.f))
    {
        return;
    }
    const float det{ e0 + e1 + e2 };
    if (det == 0.f)
    {
        return;
    }

    // Compute scaled hit distance to triangle and test against ray range
    v0t.z *= sz;
    v1t.z *= sz;
    v2t.z *= sz;
    const float t_scaled{ e0 * v0t.z + e1 * v1t.z + e2 * v2t.z };
    if (det < 0.f && (t_scaled >= 0.f || t_scaled < interval.End() * det || t_scaled > interval.Start() * det))
    {
        return;
    }
    else if (det > 0.f && (t_scaled <= 0.f || t_scaled > interval.End() * det || t_scaled < interval.Start() * det))
    {
        return;
    }

    // Compute barycentric coordinates and value for triangle intersection
    const float inv_det{ 1.f / det };
    intersection.barycentric_coordinates = Geometry::Point3f{ e0 * inv_det, e1 * inv_det, e2 * inv_det };
    interval.SetEnd(t_scaled * inv_det);

    // Set pointer
    intersection.hit_triangle = this;
}

inline bool Triangle::IntersectTest(const Geometry::Ray& ray, const Geometry::Intervalf& interval) const noexcept
{
    const Geometry::Point3f local_origin{ transformation->ToLocal(ray.Origin()) };
    const Geometry::Vector3f local_direction{ transformation->ToLocal(ray.Direction()) };

    // Translate vertices based on ray origin
    Geometry::Vector3f v0t{ mesh.VertexAt(description.v0) - local_origin };
    Geometry::Vector3f v1t{ mesh.VertexAt(description.v1) - local_origin };
    Geometry::Vector3f v2t{ mesh.VertexAt(description.v2) - local_origin };

    // Permute components of triangle vertices and ray direction
    const unsigned int kz{ Abs(local_direction).LargestDimension() };
    unsigned int kx = kz + 1;
    if (kx == 3)
    {
        kx = 0;
    }
    unsigned int ky = kx + 1;
    if (ky == 3)
    {
        ky = 0;
    }
    const Geometry::Vector3f d{ Permute(local_direction, kx, ky, kz) };
    v0t = Permute(v0t, kx, ky, kz);
    v1t = Permute(v1t, kx, ky, kz);
    v2t = Permute(v2t, kx, ky, kz);

    // Apply shear transformation to translated vertex positions
    const float sz{ 1.f / d.z };
    const float sx{ -d.x * sz };
    const float sy{ -d.y * sz };
    v0t.x += sx * v0t.z;
    v0t.y += sy * v0t.z;
    v1t.x += sx * v1t.z;
    v1t.y += sy * v1t.z;
    v2t.x += sx * v2t.z;
    v2t.y += sy * v2t.z;

    // Compute edge function coefficients e0, e1, and e2
    const float e0{ v1t.x * v2t.y - v1t.y * v2t.x };
    const float e1{ v2t.x * v0t.y - v2t.y * v0t.x };
    const float e2{ v0t.x * v1t.y - v0t.y * v1t.x };

    // Perform triangle edge and determinant tests
    if ((e0 < 0.f || e1 < 0.f || e2 < 0.f) && (e0 > 0.f || e1 > 0.f || e2 > 0.f))
    {
        return false;
    }
    const float det{ e0 + e1 + e2 };
    if (det == 0.f)
    {
        return false;
    }

    // Compute scaled hit distance to triangle and test against ray  range
    v0t.z *= sz;
    v1t.z *= sz;
    v2t.z *= sz;
    const float t_scaled{ e0 * v0t.z + e1 * v1t.z + e2 * v2t.z };
    if (det < 0.f && (t_scaled >= 0.f || t_scaled < interval.End() * det || t_scaled > interval.Start() * det))
    {
        return false;
    }
    else if (det > 0.f && (t_scaled <= 0.f || t_scaled > interval.End() * det || t_scaled < interval.Start() * det))
    {
        return false;
    }

    return true;
}

inline void Triangle::ComputeIntersectionGeometry(const Geometry::Ray& ray,
                                                  float intersection_parameter,
                                                  Geometry::TriangleIntersection& intersection) const noexcept
{
    // Compute hit point based on the input ray
    intersection.hit_point = ray(intersection_parameter);

    // Check if we have normals or we need to compute them based on vertices
    if (mesh.HasNormals())
    {
        // Compute normal based on barycentric coordinates
        intersection.local_geometry = Geometry::Framef{
            Geometry::Normalize(transformation->NormalToWorld(
                intersection.barycentric_coordinates.x * mesh.NormalAt(description.n0) +
                intersection.barycentric_coordinates.y * mesh.NormalAt(description.n1) +
                intersection.barycentric_coordinates.z * mesh.NormalAt(description.n2))) };
    }
    else
    {
        // Compute normal based on vertices
        const Geometry::Point3f& v0{ mesh.VertexAt(description.v0) };
        const Geometry::Point3f& v1{ mesh.VertexAt(description.v1) };
        const Geometry::Point3f& v2{ mesh.VertexAt(description.v2) };

        intersection.local_geometry = Geometry::Framef{
            Geometry::Normalize(transformation->NormalToWorld(Geometry::Cross(v1 - v0, v2 - v0))) };
    }

    // Set outgoing direction
    intersection.wo = Geometry::Normalize(-ray.Direction());

    // Check if we have UV coordinates
    if (mesh.HasUVs())
    {
        intersection.uv = intersection.barycentric_coordinates.x * mesh.UVAt(description.uv0) +
                          intersection.barycentric_coordinates.y * mesh.UVAt(description.uv1) +
                          intersection.barycentric_coordinates.z * mesh.UVAt(description.uv2);
    }
}

} // Rabbit namespace

#endif //RABBIT2_MESH_HPP
