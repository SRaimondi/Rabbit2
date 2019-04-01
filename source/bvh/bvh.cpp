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

BVH::BVH(const BVHConfig& config, const std::vector<Triangle>& tr)
    : configuration{ config }, triangles{ tr }
{
    Build();
}

BVH::BVH(const BVHConfig& config, std::vector<Triangle>&& tr)
    : configuration{ config }, triangles{ std::move(tr) }
{
    Build();
}

bool BVH::Intersect(Ray& ray, TriangleIntersection& intersection) const noexcept
{
#ifndef NDEBUG
    if (triangles.empty())
    {
        return false;
    }
#endif

    // Compute values needed for traversal
    const Vector3 inv_dir{ 1.f / ray.direction.x, 1.f / ray.direction.y, 1.f / ray.direction.z };
    const std::array<bool, 3> dir_is_neg{ inv_dir.x < 0.f, inv_dir.y < 0.f, inv_dir.z < 0.f };

    // Follow ray through BVH
    unsigned int to_visit_offset{ 0 }, current_node_index{ 0 };
    unsigned int nodes_to_visit[64];
    while (true)
    {
        // Get current node
        const LinearBVHNode current_node{ flat_tree_nodes[current_node_index] };
        // Check ray against node
        if (current_node.bounds.Intersect(ray, inv_dir))
        {
            // Check for leaf or interior
            if (current_node.num_triangles != 0)
            {
                for (unsigned int i = 0; i != current_node.num_triangles; i++)
                {
                    const unsigned int triangle_index{ current_node.triangle_offset + i };
                    if (triangles[triangle_index].Intersect(ray, intersection))
                    {
                        // Update index
                        intersection.triangle_index = triangle_index;
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

    // If we did hit something, fill the intersection and return true
    if (intersection.IsValid())
    {
        triangles[intersection.triangle_index].ComputeIntersectionGeometry(ray, intersection);
        return true;
    }
    else
    {
        return false;
    }
}

bool BVH::IntersectTest(const Ray& ray) const noexcept
{
#ifndef NDEBUG
    if (triangles.empty())
    {
        return false;
    }
#endif

    // Compute values needed for traversal
    const Vector3 inv_dir{ Reciprocal(ray.direction) };
    const std::array<bool, 3> dir_is_neg{ inv_dir.x < 0.f, inv_dir.y < 0.f, inv_dir.z < 0.f };

    // Follow ray through BVH
    unsigned int to_visit_offset{ 0 }, current_node_index{ 0 };
    unsigned int nodes_to_visit[64];
    while (true)
    {
        // Get current node
        const LinearBVHNode current_node{ flat_tree_nodes[current_node_index] };
        // Check ray against node
        if (current_node.bounds.Intersect(ray, inv_dir))
        {
            // Check for leaf or interior
            if (current_node.num_triangles != 0)
            {
                for (unsigned int i = 0; i != current_node.num_triangles; i++)
                {
                    const unsigned int triangle_index{ current_node.triangle_offset + i };
                    if (triangles[triangle_index].IntersectTest(ray))
                    {
                        // As soon as we hit something, return true
                        return true;
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

    // If we get here in this case, we did not hit anything
    return false;
}

void BVH::Build()
{
    // Check that the list of triangles is not empty
    if (triangles.empty())
    {
        throw std::runtime_error("Building empty BVH\n");
    }

    // Create list with information for each triangle
    std::vector<TriangleInfo> triangle_info;
    triangle_info.reserve(triangles.size());
    for (unsigned int i = 0; i != triangles.size(); i++)
    {
        triangle_info.emplace_back(i, triangles[i].Bounds());
    }

    // The building process has the freedom of swapping the triangles around such that triangles in the same leaf
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

    // If there are less triangles than the maximum, create leaf
    if (num_triangles <= configuration.max_triangles_in_leaf)
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
        PartitionResult partition_result;
        if (PartitionTriangles(triangle_info, start, end, node_bounds, partition_result))
        {
            // Recursively build tree
            return std::make_unique<BVHBuildNode>(partition_result.split_axis,
                                                  RecursiveBuild(triangle_info, start, partition_result.mid_index,
                                                                 total_nodes, ordered_triangles),
                                                  RecursiveBuild(triangle_info, partition_result.mid_index, end,
                                                                 total_nodes, ordered_triangles));
        }
        else
        {
            // Create leaf node
            const unsigned int first_triangle_offset{ static_cast<unsigned int>(ordered_triangles.size()) };
            for (unsigned int i = start; i != end; i++)
            {
                const unsigned int triangle_index{ triangle_info[i].triangle_index };
                ordered_triangles.push_back(triangles[triangle_index]);
            }

            return std::make_unique<BVHBuildNode>(first_triangle_offset, num_triangles, node_bounds);
        }
    }
}

// Helper struct to compute the cost of each bucket
struct BucketInfo
{
    BucketInfo() noexcept
        : num_triangles{ 0 }
    {}

    // Number of triangles in the bucket
    unsigned int num_triangles;
    // Bounds of the bucket
    BBox bounds;
};

bool BVH::PartitionTriangles(std::vector<TriangleInfo>& triangle_info,
                             unsigned int start, unsigned int end,
                             const BBox& node_bounds,
                             PartitionResult& partition_result) const noexcept
{
    // Compute bounds of the centroids of the triangles
    BBox centroids_bounds;
    for (unsigned int i = start; i != end; i++)
    {
        centroids_bounds = Union(centroids_bounds, triangle_info[i].centroid);
    }

    // Loop over the 3 axis and compute SAH
    std::array<std::pair<unsigned int, float>, 3> sah_axis;
    const Vector3 centroids_bounds_diagonal{ centroids_bounds.Diagonal() };
    for (unsigned int split_axis = 0; split_axis != 3; split_axis++)
    {
        if (centroids_bounds_diagonal[split_axis] != 0.f)
        {
            // Compute buckets for SAH unless bounds on that dimension are degenerate
            const std::vector<BucketInfo> buckets{ ComputeBucketsInfo(triangle_info,
                                                                      start, end,
                                                                      centroids_bounds,
                                                                      split_axis) };

            sah_axis[split_axis] = FindBestBucketIndex(buckets, node_bounds);
        }
        else
        {
            sah_axis[split_axis].second = std::numeric_limits<float>::max();
        }
    }

    // Find best split axis
    std::pair<unsigned int, float> sah_result{ sah_axis[0] };
    partition_result.split_axis = 0;
    for (unsigned int i = 1; i != 3; i++)
    {
        if (sah_axis[i].second < sah_result.second)
        {
            sah_result = sah_axis[i];
            partition_result.split_axis = i;
        }
    }

    // Check if splitting is better than creating a leaf
    const float leaf_cost{ (end - start) * configuration.triangle_intersect_cost };
    if (sah_result.second < leaf_cost)
    {
        const float split_axis_reciprocal_diagonal{ 1.f / centroids_bounds_diagonal[partition_result.split_axis] };
        const auto mid{ std::partition(triangle_info.begin() + start,
                                       triangle_info.begin() + end,
                                       [this, split_axis_reciprocal_diagonal, &sah_result,
                                           &centroids_bounds, &partition_result](const TriangleInfo& triangle_info)
                                       {
                                           // Compute index of the bucket where the triangle centroid is
                                           unsigned int bucket_index{
                                               static_cast<unsigned int>(
                                                   configuration.num_buckets *
                                                   (triangle_info.centroid[partition_result.split_axis] -
                                                    centroids_bounds.PMin()[partition_result.split_axis]) *
                                                   split_axis_reciprocal_diagonal
                                               )
                                           };

                                           // Check if we are on the end of the bounds and fix index in that case
                                           if (bucket_index == configuration.num_buckets)
                                           {
                                               bucket_index = configuration.num_buckets - 1;
                                           }

                                           return bucket_index <= sah_result.first;
                                       }) };
        partition_result.mid_index = static_cast<unsigned int>(std::distance(triangle_info.begin(), mid));

        return true;
    }
    else
    {
        // Create leaf
        return false;
    }
}

const std::vector<BucketInfo> BVH::ComputeBucketsInfo(const std::vector<TriangleInfo>& triangle_info,
                                                      unsigned int start, unsigned int end,
                                                      const BBox& centroids_bounds,
                                                      unsigned int split_axis) const noexcept
{
    std::vector<BucketInfo> buckets(configuration.num_buckets);
    const float split_axis_reciprocal_diagonal{ 1.f /
                                                (centroids_bounds.PMax()[split_axis] -
                                                 centroids_bounds.PMin()[split_axis]) };
    for (unsigned int i = start; i != end; i++)
    {
        // Compute index of the bucket where the triangle centroid is
        unsigned int bucket_index{
            static_cast<unsigned int>(
                configuration.num_buckets *
                (triangle_info[i].centroid[split_axis] - centroids_bounds.PMin()[split_axis]) *
                split_axis_reciprocal_diagonal
            )
        };

        // Check if we are on the end of the bounds and fix index in that case
        if (bucket_index == configuration.num_buckets)
        {
            bucket_index = configuration.num_buckets - 1;
        }

        // Increase count and compute bounds for the bucket
        buckets[bucket_index].num_triangles++;
        buckets[bucket_index].bounds = Union(buckets[bucket_index].bounds, triangle_info[i].bounds);
    }

    return buckets;
}

std::pair<unsigned int, float> BVH::FindBestBucketIndex(const std::vector<BucketInfo>& buckets,
                                                        const BBox& node_bounds) const noexcept
{
    // Find best bucket for split
    const unsigned int scan_size{ configuration.num_buckets - 1 };

    // Forward scan
    std::vector<BucketInfo> forward_scan(scan_size);
    forward_scan[0].bounds = buckets[0].bounds;
    forward_scan[0].num_triangles = buckets[0].num_triangles;
    for (unsigned int i = 1; i < scan_size; i++)
    {
        forward_scan[i].bounds = Union(forward_scan[i - 1].bounds, buckets[i].bounds);
        forward_scan[i].num_triangles = forward_scan[i - 1].num_triangles + buckets[i].num_triangles;
    }

    // Backward scan
    std::vector<BucketInfo> backward_scan(scan_size);
    backward_scan[0].bounds = buckets[scan_size].bounds;
    backward_scan[0].num_triangles = buckets[scan_size].num_triangles;
    for (unsigned int i = 1; i < scan_size; i++)
    {
        const unsigned int bucket_index{ scan_size - i };
        backward_scan[i].bounds = Union(backward_scan[i - 1].bounds, buckets[bucket_index].bounds);
        backward_scan[i].num_triangles = backward_scan[i - 1].num_triangles + buckets[bucket_index].num_triangles;
    }

    // Evaluate cost for splitting after each bucket
    const float inv_node_bounds_area{ 1.f / node_bounds.Surface() };
    std::vector<float> split_cost(scan_size);
    for (unsigned int i = 0; i < scan_size; i++)
    {
        split_cost[i] = configuration.bbox_intersect_cost +
                        (forward_scan[i].num_triangles *
                         forward_scan[i].bounds.Surface() +
                         backward_scan[scan_size - 1 - i].num_triangles *
                         backward_scan[scan_size - 1 - i].bounds.Surface()) *
                        inv_node_bounds_area;
    }

    // Find bucket to split that minimizes SAH
    std::pair<unsigned int, float> sah_result{ 0, split_cost[0] };
    for (unsigned int i = 1; i != scan_size; i++)
    {
        if (split_cost[i] < sah_result.second)
        {
            sah_result.second = split_cost[i];
            sah_result.first = i;
        }
    }

    return sah_result;
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
