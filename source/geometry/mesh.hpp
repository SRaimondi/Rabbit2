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
namespace Geometry
{

class Triangle;

class Mesh
{
public:
    Mesh(std::vector<Point3f>&& v, std::vector<Vector3f>&& n, std::vector<unsigned int>&& i);

    const Point3f& VertexAt(unsigned int vertex_index) const
    {
        assert(vertex_index < vertices.size());
        return vertices[vertex_index];
    }

    const Vector3f& NormalAt(unsigned int normal_index) const
    {
        assert(normal_index < normals.size());
        return normals[normal_index];
    }

    unsigned int FaceIndexAt(unsigned int index) const
    {
        assert(index < indices.size());
        return indices[index];
    }

    // Load mesh from .obj
    static Mesh LoadOBJ(const std::string& filename);

    // Load mesh from .ply
    static Mesh LoadPLY(const std::string& filename);

    // Create list of triangles for the mesh
    std::vector<Triangle> CreateTriangles(const std::shared_ptr<const Transform>& transform) const;

private:
    friend std::ostream& operator<<(std::ostream& os, const Mesh& mesh);

    // Compute smooth normals for a set of vertices and indices
    static std::vector<Vector3f> SmoothNormals(const std::vector<Point3f>& vertices,
                                               const std::vector<unsigned int>& indices);

    // Mesh representation
    std::vector<Point3f> vertices;
    std::vector<Vector3f> normals;
    std::vector<unsigned int> indices;
};

std::ostream& operator<<(std::ostream& os, const Mesh& mesh);

class Triangle
{
public:
    Triangle(unsigned int fi, const Mesh& m, const std::shared_ptr<const Transform>& transform) noexcept;

    // Compute triangle BBox in world space
    const BBox Bounds() const noexcept
    {
        return { transformation->ToWorld(mesh.VertexAt(mesh.FaceIndexAt(first_index))),
                 transformation->ToWorld(mesh.VertexAt(mesh.FaceIndexAt(first_index + 1))),
                 transformation->ToWorld(mesh.VertexAt(mesh.FaceIndexAt(first_index + 2))) };
    }

    // Intersect ray with triangle
    bool Intersect(const Ray& ray, Intervalf& interval, TriangleIntersection& intersection) const noexcept;

    // Check for intersection
    bool IntersectTest(const Ray& ray, const Intervalf& interval) const noexcept;

    // Fill geometry information, ray is assumed to be in world space so we can compute the hit point directly
    void ComputeIntersectionGeometry(const Ray& ray,
                                     float intersection_parameter,
                                     TriangleIntersection& intersection) const noexcept;

private:
    // Index of the first vertex
    const unsigned int first_index;
    // Mesh associated
    const Mesh& mesh;
    // Transformation for the triangle
    std::shared_ptr<const Transform> transformation;
};

inline bool Triangle::Intersect(const Ray& ray, Intervalf& interval, TriangleIntersection& intersection) const noexcept
{
    const Point3f local_origin{ transformation->ToLocal(ray.origin) };
    const Vector3f local_direction{ transformation->ToLocal(ray.direction) };

    // Translate vertices based on ray origin
    Vector3f v0t{ mesh.VertexAt(mesh.FaceIndexAt(first_index)) - local_origin };
    Vector3f v1t{ mesh.VertexAt(mesh.FaceIndexAt(first_index + 1)) - local_origin };
    Vector3f v2t{ mesh.VertexAt(mesh.FaceIndexAt(first_index + 2)) - local_origin };

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
    const Vector3f d{ Permute(local_direction, kx, ky, kz) };
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
    if (det < 0.f && (t_scaled >= 0.f || t_scaled < interval.end * det || t_scaled > interval.start * det))
    {
        return false;
    }
    else if (det > 0.f && (t_scaled <= 0.f || t_scaled > interval.end * det || t_scaled < interval.start * det))
    {
        return false;
    }

    // Compute barycentric coordinates and value for triangle intersection
    const float inv_det{ 1.f / det };
    intersection.u = e0 * inv_det;
    intersection.v = e1 * inv_det;
    intersection.w = e2 * inv_det;
    interval.end = t_scaled * inv_det;

    return true;
}

inline bool Triangle::IntersectTest(const Ray& ray, const Intervalf& interval) const noexcept
{
    const Point3f local_origin{ transformation->ToLocal(ray.origin) };
    const Vector3f local_direction{ transformation->ToLocal(ray.direction) };

    // Translate vertices based on ray origin
    Vector3f v0t{ mesh.VertexAt(mesh.FaceIndexAt(first_index)) - local_origin };
    Vector3f v1t{ mesh.VertexAt(mesh.FaceIndexAt(first_index + 1)) - local_origin };
    Vector3f v2t{ mesh.VertexAt(mesh.FaceIndexAt(first_index + 2)) - local_origin };

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
    const Vector3f d{ Permute(local_direction, kx, ky, kz) };
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
    if (det < 0.f && (t_scaled >= 0.f || t_scaled < interval.end * det || t_scaled > interval.start * det))
    {
        return false;
    }
    else if (det > 0.f && (t_scaled <= 0.f || t_scaled > interval.end * det || t_scaled < interval.start * det))
    {
        return false;
    }

    return true;
}

inline void Triangle::ComputeIntersectionGeometry(const Ray& ray,
                                                  float intersection_parameter,
                                                  TriangleIntersection& intersection) const noexcept
{
    // Compute hit point based on the input ray
    intersection.hit_point = ray(intersection_parameter);
    // Compute normal based on barycentric coordinates
    intersection.normal = Normalize(transformation->NormalToWorld(
        intersection.u * mesh.NormalAt(mesh.FaceIndexAt(first_index)) +
        intersection.v * mesh.NormalAt(mesh.FaceIndexAt(first_index + 1)) +
        intersection.w * mesh.NormalAt(mesh.FaceIndexAt(first_index + 2))));
}

} // Geometry namespace
} // Rabbit namespace

#endif //RABBIT2_MESH_HPP
