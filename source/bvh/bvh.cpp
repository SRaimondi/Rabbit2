//
// Created by Simon on 2019-03-28.
//

#include "bvh.hpp"

#include <iostream>

namespace Geometry
{

// BVH building node
struct BVHBuildNode
{
    // Create leaf node
    BVHBuildNode(unsigned int first_triangle, unsigned int num_triangles, const BBox& bounds) noexcept
        : first_triangle_index{ first_triangle }, num_triangles{ num_triangles }, bounds{ bounds }
    {}

    // Create internal node
    BVHBuildNode(unsigned int split_axis, std::unique_ptr<BVHBuildNode> c0, std::unique_ptr<BVHBuildNode> c1) noexcept
        : children{ std::move(c0), std::move(c1) }, split_axis{ split_axis }
    {}

    // Index of the first triangle and number of triangles in it
    unsigned int first_triangle_index, num_triangles;
    // Bounds of the node
    BBox bounds;
    // Axis used for the split
    unsigned int split_axis;
    // Children nodes
    std::array<std::unique_ptr<BVHBuildNode>, 2> children;
};

BVH::BVH(const BVHConfig& config, const std::vector<Triangle>& tr)
    : configuration{ config }, triangles{ tr }
{
    // Check that the list of triangles is not empty
    if (triangles.empty())
    {
        std::cout << "Triangle list given to BVH is empty\n";
        return;
    }

    // Create list with information for each triangle
    std::vector<TriangleInfo> triangle_info;
    triangle_info.reserve(triangles.size());
    for (unsigned int i = 0; i != triangles.size(); i++)
    {
        triangle_info.emplace_back(i, triangles[i].Bounds());
    }


}

} // Geometry namespace
