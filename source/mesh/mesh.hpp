//
// Created by Simon on 2019-03-26.
//

#ifndef RABBIT2_MESH_HPP
#define RABBIT2_MESH_HPP

#include "geometry/bbox.hpp"
#include "geometry/intersection.hpp"
#include "geometry/transform.hpp"

#include <vector>
#include <ostream>
#include <memory>

namespace Rabbit
{

class Triangle;

class Mesh
{
public:
    Mesh(std::vector<Geometry::Point3f>&& v, std::vector<Geometry::Vector3f>&& n, std::vector<unsigned int>&& i);

    const Geometry::Point3f& VertexAt(unsigned int vertex_index) const
    {
        assert(vertex_index < vertices.size());
        return vertices[vertex_index];
    }

    const Geometry::Vector3f& NormalAt(unsigned int normal_index) const
    {
        assert(normal_index < normals.size());
        return normals[normal_index];
    }

    unsigned int FaceIndexAt(unsigned int index) const
    {
        assert(index < indices.size());
        return indices[index];
    }

    // Create list of triangles for the mesh
    std::vector<Triangle> CreateTriangles(const std::shared_ptr<const Geometry::Transform>& transform) const;

private:
    // Mesh representation
    std::vector<Geometry::Point3f> vertices;
    std::vector<Geometry::Vector3f> normals;
    std::vector<Geometry::Vector2f> uvs;
    std::vector<unsigned int> indices;
};

std::ostream& operator<<(std::ostream& os, const Mesh& mesh);

class Triangle
{
public:
    Triangle(unsigned int fi, const Mesh& m, const std::shared_ptr<const Geometry::Transform>& transform) noexcept;

    // Compute triangle BBox in world space
    const Geometry::BBox Bounds() const noexcept
    {
        return { transformation->ToWorld(mesh.VertexAt(mesh.FaceIndexAt(first_index))),
                 transformation->ToWorld(mesh.VertexAt(mesh.FaceIndexAt(first_index + 1))),
                 transformation->ToWorld(mesh.VertexAt(mesh.FaceIndexAt(first_index + 2))) };
    }

    // Intersect ray with triangle
    bool Intersect(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                   Geometry::TriangleIntersection& intersection) const noexcept;

    // Check for intersection
    bool IntersectTest(const Geometry::Ray& ray, const Geometry::Intervalf& interval) const noexcept;

    // Fill geometry information, ray is assumed to be in world space so we can compute the hit point directly
    void ComputeIntersectionGeometry(const Geometry::Ray& ray,
                                     float intersection_parameter,
                                     Geometry::TriangleIntersection& intersection) const noexcept;

private:
    // Index of the first vertex
    const unsigned int first_index;
    // Mesh associated
    const Mesh& mesh;
    // Transformation for the triangle
    std::shared_ptr<const Geometry::Transform> transformation;
};

inline bool Triangle::Intersect(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                                Geometry::TriangleIntersection& intersection) const noexcept
{
    const Geometry::Point3f local_origin{ transformation->ToLocal(ray.Origin()) };
    const Geometry::Vector3f local_direction{ transformation->ToLocal(ray.Direction()) };

    // Translate vertices based on ray origin
    Geometry::Vector3f v0t{ mesh.VertexAt(mesh.FaceIndexAt(first_index)) - local_origin };
    Geometry::Vector3f v1t{ mesh.VertexAt(mesh.FaceIndexAt(first_index + 1)) - local_origin };
    Geometry::Vector3f v2t{ mesh.VertexAt(mesh.FaceIndexAt(first_index + 2)) - local_origin };

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

    // Compute scaled hit distance to triangle and test against ray range
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

    // Compute barycentric coordinates and value for triangle intersection
    const float inv_det{ 1.f / det };
    intersection.u = e0 * inv_det;
    intersection.v = e1 * inv_det;
    intersection.w = e2 * inv_det;
    interval.SetEnd(t_scaled * inv_det);

    return true;
}

inline bool Triangle::IntersectTest(const Geometry::Ray& ray, const Geometry::Intervalf& interval) const noexcept
{
    const Geometry::Point3f local_origin{ transformation->ToLocal(ray.Origin()) };
    const Geometry::Vector3f local_direction{ transformation->ToLocal(ray.Direction()) };

    // Translate vertices based on ray origin
    Geometry::Vector3f v0t{ mesh.VertexAt(mesh.FaceIndexAt(first_index)) - local_origin };
    Geometry::Vector3f v1t{ mesh.VertexAt(mesh.FaceIndexAt(first_index + 1)) - local_origin };
    Geometry::Vector3f v2t{ mesh.VertexAt(mesh.FaceIndexAt(first_index + 2)) - local_origin };

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
    // Compute normal based on barycentric coordinates
    intersection.normal = Normalize(transformation->NormalToWorld(
        intersection.u * mesh.NormalAt(mesh.FaceIndexAt(first_index)) +
        intersection.v * mesh.NormalAt(mesh.FaceIndexAt(first_index + 1)) +
        intersection.w * mesh.NormalAt(mesh.FaceIndexAt(first_index + 2))));
}

} // Rabbit namespace

#endif //RABBIT2_MESH_HPP