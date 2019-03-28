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
    BVHBuildNode(unsigned int first_triangle_o, unsigned int num_triangles, const BBox& bounds) noexcept
        : first_triangle_offset{ first_triangle_o }, num_triangles{ num_triangles }, bounds{ bounds }
    {}

    // Create internal node
    BVHBuildNode(unsigned int split_axis, std::unique_ptr<BVHBuildNode> c0, std::unique_ptr<BVHBuildNode> c1) noexcept
        : children{ std::move(c0), std::move(c1) }, split_axis{ split_axis }
    {}

    // Offset of the first triangle and number of triangles in it
    unsigned int first_triangle_offset, num_triangles;
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

    // The building process has the freed of swapping the triangles around such that triangles in the same leaf
    // are also close in memory
    std::vector<Triangle> ordered_triangles;
    ordered_triangles.reserve(triangles.size());

    // Start building
    unsigned int total_nodes{ 0 };
    const std::unique_ptr<BVHBuildNode> root{ RecursiveBuild(triangle_info,
                                                             0, static_cast<unsigned int>(triangle_info.size()),
                                                             total_nodes,
                                                             ordered_triangles) };

    // Move the content of the ordered_triangles in the local triangles
    triangles = std::move(ordered_triangles);
}

std::unique_ptr<BVHBuildNode> BVH::RecursiveBuild(std::vector<TriangleInfo>& triangle_info,
                                                  unsigned int start, unsigned int end,
                                                  unsigned int& total_nodes,
                                                  std::vector<Triangle>& ordered_triangles) noexcept
{
    // Increase by 1 the number of created nodes
    total_nodes++;

    // First thing we do is compute the BBox of all the triangles in this node
    BBox node_bounds;
    for (unsigned int i = start; i != end; i++)
    {
        node_bounds = Union(node_bounds, triangle_info[i].bounds);
    }

    // Compute number of triangles for this node
    const unsigned int num_triangles{ end - start };

    // If there are less triangles than the maximum for a leaf, create it and return it
    if (num_triangles < configuration.max_triangles_in_leaf)
    {
        const unsigned int first_triangle_offset{ static_cast<unsigned int>(ordered_triangles.size()) };
        for (unsigned int i = start; i != end; i++)
        {
            const unsigned int triangle_index{ triangle_info[i].triangle_index };
            ordered_triangles.push_back(triangles[triangle_index]);
        }
        return std::make_unique<BVHBuildNode>(first_triangle_offset, num_triangles, node_bounds);
    }
    else
    {
        // Compute bounds of the centroids
        BBox centroids_bounds;
        for (unsigned int i = start; i != end; i++)
        {
            centroids_bounds = Union(centroids_bounds, triangle_info[i].centroid);
        }

        // TODO Choose splitting dimension
        unsigned int split_axis{ 0 };


        // TODO Partition triangles
        unsigned int mid{ (start + end) / 2 };

        // Recursively build tree
        return std::make_unique<BVHBuildNode>(split_axis,
                                              RecursiveBuild(triangle_info, start, mid, total_nodes, ordered_triangles),
                                              RecursiveBuild(triangle_info, mid, end, total_nodes, ordered_triangles));
    }
}

} // Geometry namespace
