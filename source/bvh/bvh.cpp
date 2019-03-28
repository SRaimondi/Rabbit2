//
// Created by Simon on 2019-03-28.
//

#include "bvh.hpp"

#include <iostream>

namespace Geometry
{

BVH::BVH(const BVHConfig& config, const std::vector<Triangle>& tr)
    : configuration{ config }, triangles{ tr }
{
    // Check that the list of triangles is not empty
    if (triangles.empty())
    {
        std::cout << "Triangle list given to BVH is empty\n";
        return;
    }

    // Create list with the information for each triangle
    std::vector<TriangleInfo> triangle_info;
    triangle_info.reserve(triangles.size());
    for (unsigned int i = 0; i != triangles.size(); i++)
    {
        triangle_info.emplace_back(i, triangles[i].Bounds());
    }
}

} // Geometry namespace
