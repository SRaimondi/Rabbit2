//
// Created by Simon on 2019-03-28.
//

#ifndef RABBIT2_BVH_HPP
#define RABBIT2_BVH_HPP

#include "geometry/mesh.hpp"

#include <memory>

namespace Rabbit
{

// Information of a triangle for building
struct TriangleInfo
{
    constexpr TriangleInfo(unsigned int index, const Geometry::BBox& b) noexcept
        : triangle_index{ index }, bounds{ b }, centroid{ bounds.Centroid() }
    {}

    // Index of the triangle in the list
    unsigned int triangle_index;
    Geometry::BBox bounds;
    Geometry::Point3f centroid;
};

// Linear BVH node with 32 byte size for optimal cache performance
struct alignas(32) LinearBVHNode
{
    Geometry::BBox bounds;              // 24 bytes
    union                               // 4 bytes
    {
        uint32_t triangle_offset;       // For leafs
        uint32_t second_child_offset;   // Interior node
    };
    uint16_t num_triangles;             // 2 bytes, 0 for interior node
    uint8_t split_axis;                 // 1 byte
    uint8_t padding[1];                 // 1 byte padding to ensure alignment
};

// Configuration of the BVH
struct BVHConfig
{
    constexpr explicit BVHConfig(unsigned int max_triangle_in_leaf = 4,
                                 float triangle_intersect_cost = 1.f,
                                 float bbox_intersect_cost = 0.2f,
                                 unsigned int num_buckets = 12) noexcept
        : max_triangles_in_leaf{ std::min(255u, max_triangle_in_leaf) },
          triangle_intersect_cost{ triangle_intersect_cost },
          bbox_intersect_cost{ bbox_intersect_cost },
          num_buckets{ std::max(2u, num_buckets) }
    {}

    // Maximum number of triangles in leaf node
    const unsigned int max_triangles_in_leaf;
    // Cost of intersecting a triangle
    const float triangle_intersect_cost;
    // BBox intersect cost
    const float bbox_intersect_cost;
    // Number of buckets to use to evaluate SAH
    const unsigned int num_buckets;
};

// Output of the PartitionTriangles function
struct PartitionResult
{
    PartitionResult() noexcept
        : split_axis{ 3 }, mid_index{ 0 }
    {}

    // Split axis
    unsigned int split_axis;
    // Index of the midpoint for the two partitions
    unsigned int mid_index;
};

// Forward declare build node struct
struct BVHBuildNode;
struct BucketInfo;

class BVH
{
public:
    BVH(const BVHConfig& config, const std::vector<Geometry::Triangle>& tr);

    BVH(const BVHConfig& config, std::vector<Geometry::Triangle>&& tr);

    // Intersect Ray with BVH
    bool Intersect(Geometry::Ray& ray, Geometry::TriangleIntersection& intersection) const noexcept;

    // Check for intersection
    bool IntersectTest(const Geometry::Ray& ray) const noexcept;

private:
    // Build tree
    void Build();

    // Recursively build a subpart of the tree for the given range of triangles start to end (not included)
    std::unique_ptr<BVHBuildNode> RecursiveBuild(std::vector<TriangleInfo>& triangle_info,
                                                 unsigned int start, unsigned int end,
                                                 unsigned int& total_nodes,
                                                 std::vector<Geometry::Triangle>& ordered_triangles) noexcept;

    // Partition triangles in current range, the two partitions are going to be [start, mid) and [mid, end)
    bool PartitionTriangles(std::vector<TriangleInfo>& triangle_info,
                            unsigned int start, unsigned int end,
                            const Geometry::BBox& node_bounds,
                            PartitionResult& partition_result) const noexcept;

    // Compute buckets information for SAH
    const std::vector<BucketInfo> ComputeBucketsInfo(const std::vector<TriangleInfo>& triangle_info,
                                                     unsigned int start, unsigned int end,
                                                     const Geometry::BBox& centroids_bounds,
                                                     unsigned int split_axis) const noexcept;

    // Find index of the best bucket to split with the cost
    std::pair<unsigned int, float> FindBestBucketIndex(const std::vector<BucketInfo>& buckets,
                                                       const Geometry::BBox& node_bounds) const noexcept;

    // Flatten out tree
    unsigned int FlattenTree(const std::unique_ptr<BVHBuildNode>& node, unsigned int& offset) noexcept;

    // BVH members
    const BVHConfig configuration;
    std::vector<Geometry::Triangle> triangles;
    std::vector<LinearBVHNode> flat_tree_nodes;
};

} // Rabbit namespace

#endif //RABBIT2_BVH_HPP
