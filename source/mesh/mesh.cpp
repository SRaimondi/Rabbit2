//
// Created by Simon on 2019-03-26.
//

#include "mesh.hpp"

namespace Rabbit
{

Mesh::Mesh(std::vector<Geometry::Point3f>&& v, std::vector<Geometry::Vector3f>&& n, std::vector<unsigned int>&& i)
    : vertices{ std::move(v) }, normals{ std::move(n) }, indices{ std::move(i) }
{}

std::vector<Triangle> Mesh::CreateTriangles(const std::shared_ptr<const Geometry::Transform>& transform) const
{
    std::vector<Triangle> triangles;
    triangles.reserve(indices.size() / 3);
    for (unsigned int index = 0; index != indices.size(); index += 3)
    {
        triangles.emplace_back(index, *this, transform);
    }

    return triangles;
}

Triangle::Triangle(unsigned int fi, const Mesh& m, const std::shared_ptr<const Geometry::Transform>& transform) noexcept
    : first_index{ fi }, mesh{ m }, transformation{ transform }
{}

} // Rabbit namespace
