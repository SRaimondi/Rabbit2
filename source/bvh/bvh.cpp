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
        : first_triangle_offset{ first_triangle_o }, num_triangles{ num_triangles }, split_axis{ 3 }, bounds{ bounds }
    {}

    // Create internal node
    BVHBuildNode(unsigned int split_axis, std::unique_ptr<BVHBuildNode> c0, std::unique_ptr<BVHBuildNode> c1) noexcept
        : first_triangle_offset{ 0 }, num_triangles{ 0 },
          split_axis{ split_axis }, children{ std::move(c0), std::move(c1) },
          bounds{ Union(children[0]->bounds, children[1]->bounds) }
    {}

    // Offset of the first triangle and number of triangles in it
    unsigned int first_triangle_offset, num_triangles;
    // Axis used for the split
    unsigned int split_axis;
    // Children nodes
    std::array<std::unique_ptr<BVHBuildNode>, 2> children;
    // Bounds of the node
    BBox bounds;
};

BVH::BVH(const BVHConfig& config, std::vector<Triangle>&& tr)
    : configuration{ config }, triangles{ std::move(tr) }
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

    // Bring tree into flat representation
    unsigned int offset{ 0 };
    flat_tree_nodes.resize(total_nodes);
    FlattenTree(root, offset);
    assert(offset == total_nodes);
}

bool BVH::Intersect(Ray& ray, TriangleIntersection& intersection) const noexcept
{
#ifndef NDEBUG
    if (triangles.empty())
    {
        return false;
    }
#endif

    bool hit{ false };
    // Compute values needed for traversal
    const Vector3 inv_dir{ 1.f / ray.direction.x, 1.f / ray.direction.y, 1.f / ray.direction.z };
    const unsigned int dir_is_neg[3]{ ray.direction.x < 0.f, ray.direction.y < 0.f, ray.direction.z < 0.f };

    // Follow ray through BVH
    unsigned int to_visit_offset{ 0 };
    unsigned int current_node_index{ 0 };
    unsigned int nodes_to_visit[64];
    while (true)
    {
        // Get current node
        const LinearBVHNode current_node{ flat_tree_nodes[current_node_index] };
        // Check ray against node
        if (current_node.bounds.Intersect(ray, inv_dir, dir_is_neg))
        {
            // Check for leaf or interior
            if (current_node.num_triangles != 0)
            {
                for (unsigned int i = 0; i != current_node.num_triangles; i++)
                {
                    if (triangles[current_node.triangle_offset + i].Intersect(ray, intersection))
                    {
                        // Set hit and update index
                        hit = true;
                        intersection.triangle_index = current_node.triangle_offset + i;
                    }
                }
                // Check if we still have node to visit
                if (to_visit_offset == 0)
                {
                    break;
                }
                current_node_index = nodes_to_visit[--to_visit_offset];
            }
            else
            {
                // Put far node on stack and visit closest one
                if (dir_is_neg[current_node.split_axis])
                {
                    nodes_to_visit[to_visit_offset++] = current_node_index + 1;
                    current_node_index = current_node.second_child_offset;
                }
                else
                {
                    nodes_to_visit[to_visit_offset++] = current_node.second_child_offset;
                    current_node_index++;
                }
            }
        }
        else
        {
            // Check if we still have node to visit
            if (to_visit_offset == 0)
            {
                break;
            }
            current_node_index = nodes_to_visit[--to_visit_offset];
        }
    }

    // If we did hit something, fill the intersection
    if (hit)
    {
        triangles[intersection.triangle_index].ComputeIntersectionGeometry(ray, intersection);
    }

    return hit;
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
        // Split triangles
        const PartitionResult partition_result{ PartitionTriangles(triangle_info, start, end) };
        // Recursively build tree
        return std::make_unique<BVHBuildNode>(partition_result.split_axis,
                                              RecursiveBuild(triangle_info, start, partition_result.mid_index,
                                                             total_nodes, ordered_triangles),
                                              RecursiveBuild(triangle_info, partition_result.mid_index, end,
                                                             total_nodes, ordered_triangles));
    }
}


PartitionResult BVH::PartitionTriangles(std::vector<TriangleInfo>& triangle_info,
                                        unsigned int start, unsigned int end) noexcept
{
    // Compute bounds of the centroids of the triangles
    BBox centroids_bounds;
    for (unsigned int i = start; i != end; i++)
    {
        centroids_bounds = Union(centroids_bounds, triangle_info[i].centroid);
    }

    // TODO Choose splitting dimension
    unsigned int split_axis{ centroids_bounds.LargestDimension() };

    // TODO Partition triangles based on midpoint
    const float axis_mid_point{ centroids_bounds.Centroid()[split_axis] };
    const auto mid_iter{ std::partition(triangle_info.begin() + start,
                                        triangle_info.begin() + end,
                                        [split_axis, axis_mid_point](const TriangleInfo& triangle_info) -> bool
                                        {
                                            return triangle_info.centroid[split_axis] < axis_mid_point;
                                        }) };

    return { split_axis, static_cast<unsigned int>(std::distance(triangle_info.begin(), mid_iter)) };
}

unsigned int BVH::FlattenTree(const std::unique_ptr<BVHBuildNode>& node, unsigned int& offset) noexcept
{
    // Store current offset
    const unsigned int my_offset{ offset++ };

    // Get reference to the node we use
    LinearBVHNode& linear_node = flat_tree_nodes[my_offset];
    // Set the bounds of the node
    linear_node.bounds = node->bounds;

    // Depending on the number of triangles, either create a leaf or an interior node
    if (node->num_triangles > 0)
    {
        // Create leaf
        assert(node->split_axis == 3);
        linear_node.triangle_offset = node->first_triangle_offset;
        linear_node.num_triangles = static_cast<uint16_t>(node->num_triangles);
    }
    else
    {
        // Create interior
        assert(node->split_axis < 3);
        linear_node.split_axis = static_cast<uint8_t>(node->split_axis);
        linear_node.num_triangles = 0;
        // Recursively call the flatten procedure on the first child
        FlattenTree(node->children[0], offset);
        // Recursively call the flatten procedure on the second child and store his offset
        linear_node.second_child_offset = FlattenTree(node->children[1], offset);
    }

    return my_offset;
}

} // Geometry namespace
