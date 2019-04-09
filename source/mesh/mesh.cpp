//
// Created by Simon on 2019-03-26.
//

#include "mesh.hpp"

namespace Rabbit
{

Mesh::Mesh(std::vector<Geometry::Point3f>&& v, std::vector<Geometry::Vector3f>&& n,
           std::vector<Geometry::Vector2f>&& uvs, std::vector<TriangleDescription>&& tr)
    : vertices{ std::move(v) }, normals{ std::move(n) }, uvs{ std::move(uvs) }, triangles{ std::move(tr) }
{}

std::vector<Triangle> Mesh::CreateTriangles(const std::shared_ptr<const Geometry::Transform>& transform,
                                            const std::shared_ptr<const MaterialInterface>& material) const noexcept
{
    std::vector<Triangle> mesh_triangles;
    mesh_triangles.reserve(triangles.size());
    for (const TriangleDescription& triangle : triangles)
    {
        mesh_triangles.emplace_back(triangle, *this, transform, material);
    }

    return mesh_triangles;
}

void Mesh::CreateTriangles(const std::shared_ptr<const Geometry::Transform>& transform,
                           const std::shared_ptr<const MaterialInterface>& material,
                           std::vector<Triangle>& triangles_list) const noexcept
{
    for (const TriangleDescription& triangle: triangles)
    {
        triangles_list.emplace_back(triangle, *this, transform, material);
    }
}

Triangle::Triangle(const TriangleDescription& description, const Mesh& m,
                   const std::shared_ptr<const Geometry::Transform>& transform,
                   const std::shared_ptr<const MaterialInterface>& material) noexcept
    : material{ material }, description{ description }, mesh{ m }, transformation{ transform }
{}

} // Rabbit namespace
