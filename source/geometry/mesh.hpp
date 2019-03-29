//
// Created by Simon on 2019-03-26.
//

#ifndef RABBIT2_MESH_HPP
#define RABBIT2_MESH_HPP

#include "geometry/bbox.hpp"
#include "geometry/intersection.hpp"

#include <vector>
#include <ostream>

namespace Geometry
{

class Triangle;

class Mesh
{
public:
    Mesh(std::vector<Vector3>&& v, std::vector<Vector3>&& n, std::vector<unsigned int>&& i);

    const Vector3& VertexAt(unsigned int vertex_index) const
    {
        assert(vertex_index < vertices.size());
        return vertices[vertex_index];
    }

    const Vector3& NormalAt(unsigned int normal_index) const
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

    // Create list of triangles for the mesh
    std::vector<Triangle> CreateTriangles() const;

private:
    friend std::ostream& operator<<(std::ostream& os, const Mesh& mesh);

    // Compute smooth normals for a set of vertices and indices
    static std::vector<Vector3> SmoothNormals(const std::vector<Vector3>& vertices,
                                              const std::vector<unsigned int>& indices);

    // Mesh representation
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<unsigned int> indices;
};

std::ostream& operator<<(std::ostream& os, const Mesh& mesh);

class Triangle
{
public:
    Triangle(unsigned int fi, const Mesh& m) noexcept;

    // Compute triangle BBox
    const BBox Bounds() const noexcept
    {
        return { mesh.VertexAt(mesh.FaceIndexAt(first_index)),
                 mesh.VertexAt(mesh.FaceIndexAt(first_index + 1)),
                 mesh.VertexAt(mesh.FaceIndexAt(first_index + 2)) };
    }

    // Intersect ray with triangle
    bool Intersect(Ray& ray, TriangleIntersection& intersection) const noexcept;

    // Fill geometry information
    void ComputeIntersectionGeometry(const Ray& ray,
                                     TriangleIntersection& intersection) const noexcept;

private:
    // Index of the first vertex
    const unsigned int first_index;
    // Mesh associated
    const Mesh& mesh;
};

// From "Fast􏰁 Minimum Storage Ray􏰂Triangle Intersection" - Tomas Mö􏰃ller
inline bool Triangle::Intersect(Ray& ray, TriangleIntersection& intersection) const noexcept
{
    // Tolerance value used in intersection process
    constexpr float EPSILON{ 0.0001f };

    // Access triangle vertices
    const Vector3 v0{ mesh.VertexAt(mesh.FaceIndexAt(first_index)) };
    const Vector3 v1{ mesh.VertexAt(mesh.FaceIndexAt(first_index + 1)) };
    const Vector3 v2{ mesh.VertexAt(mesh.FaceIndexAt(first_index + 2)) };

    // Compute triangle edges
    const Vector3 edge1{ v1 - v0 };
    const Vector3 edge2{ v2 - v0 };

    // Compute determinant
    const Vector3 pvec{ Cross(ray.direction, edge2) };
    const float determinant{ Dot(edge1, pvec) };

    // Check if ray is parallel to triangle
    if (determinant > -EPSILON && determinant < EPSILON)
    {
        return false;
    }
    const float inv_determinant{ 1.f / determinant };

    // Calculate u parameter and test bounds
    const Vector3 tvec{ ray.origin - v0 };
    intersection.u = inv_determinant * Dot(tvec, pvec);
    if (intersection.u < 0.f || intersection.u > 1.f)
    {
        return false;
    }

    // Calculate v parameter and test bounds
    const Vector3 qvec{ Cross(tvec, edge1) };
    intersection.v = inv_determinant * Dot(ray.direction, qvec);
    if (intersection.v < 0.f || intersection.u + intersection.v > 1.f)
    {
        return false;
    }

    // Calculate t (ray intersection parameter)
    const float t{ inv_determinant * Dot(edge2, qvec) };
    if (!ray.IsInRange(t))
    {
        return false;
    }

    // We did hit this triangle, update ray maximum extent
    ray.extent_end = t;

    return true;
}

inline void Triangle::ComputeIntersectionGeometry(const Ray& ray,
                                                  TriangleIntersection& intersection) const noexcept
{
    // Compute hit point based on the input ray
    intersection.hit_point = ray(ray.extent_end);
    // Compute normal based on barycentric coordinates
    intersection.normal = (1.f - intersection.u - intersection.v) * mesh.NormalAt(mesh.FaceIndexAt(first_index)) +
                          intersection.u * mesh.NormalAt(mesh.FaceIndexAt(first_index + 1)) +
                          intersection.v * mesh.NormalAt(mesh.FaceIndexAt(first_index + 2));
}

} // Geometry namespace

#endif //RABBIT2_MESH_HPP
