//
// Created by Simon on 2019-03-28.
//

#ifndef RABBIT2_BVH_HPP
#define RABBIT2_BVH_HPP

#include "geometry/mesh.hpp"

namespace Geometry
{

// Information of a triangle for building
struct TriangleInfo
{
    constexpr TriangleInfo(unsigned int index, const BBox& b) noexcept
        : triangle_index{ index }, bounds{ b }, centroid{ bounds.Centroid() }
    {}

    // Index of the triangle in the list
    unsigned int triangle_index;
    BBox bounds;
    Vector3 centroid;
};

// Configuration of the BVH
struct BVHConfig
{
    constexpr explicit BVHConfig(unsigned int max_triangle_in_leaf = 4,
                                 float triangle_intersect_cost = 1.f,
                                 float bbox_intersect_cost = 0.125f,
                                 unsigned int num_buckets = 12) noexcept
        : max_triangles_in_leaf{ max_triangle_in_leaf },
          triangle_intersect_cost{ triangle_intersect_cost },
          bbox_intersect_cost{ bbox_intersect_cost },
          num_buckets{ num_buckets }
    {}

    // Maximum number of triangles in leaf node
    const unsigned int max_triangles_in_leaf;
    // Cost of intersecting a triangle
    const float triangle_intersect_cost;
    // BBox intersect cost
    const float bbox_intersect_cost;
    // Number of buckets to use to evaluate heuristic
    const unsigned int num_buckets;
};

struct BVHBuildNode;

class BVH
{
public:
    BVH(const BVHConfig& config, const std::vector<Triangle>& tr);

private:
    // Recursively build a subpart of the tree for the given range of triangles start to end (not included)
    std::unique_ptr<BVHBuildNode> RecursiveBuild(std::vector<TriangleInfo>& triangle_info,
                                                 unsigned int start, unsigned int end,
                                                 unsigned int& total_nodes,
                                                 std::vector<Triangle>& ordered_triangles) noexcept;

    const BVHConfig configuration;
    std::vector<Triangle> triangles;
};

} // Geometry namespace

#endif //RABBIT2_BVH_HPP
