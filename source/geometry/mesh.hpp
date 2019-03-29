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

private:
    // Index of the first vertex
    const unsigned int first_index;
    // Mesh associated
    const Mesh& mesh;
};

inline bool Triangle::Intersect(Ray& ray, TriangleIntersection& intersection) const noexcept
{
    // Access triangle vertices
    const Vector3 v0{ mesh.VertexAt(mesh.FaceIndexAt(first_index)) };
    const Vector3 v1{ mesh.VertexAt(mesh.FaceIndexAt(first_index + 1)) };
    const Vector3 v2{ mesh.VertexAt(mesh.FaceIndexAt(first_index + 2)) };

    // Compute triangle edges
    const Vector3 e0{ v1 - v0 };
    const Vector3 e1{ v2 - v0 };

    // Compute determinant

}

} // Geometry namespace

#endif //RABBIT2_MESH_HPP
