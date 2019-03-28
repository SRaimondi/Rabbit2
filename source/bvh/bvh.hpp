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
    constexpr TriangleInfo(size_t index, const BBox& b) noexcept
        : triangle_index{ index }, bounds{ b }, centroid{ bounds.Centroid() }
    {}

    // Index of the triangle in the list
    size_t triangle_index;
    BBox bounds;
    Vector3 centroid;
};

// Configuration of the BVH
struct BVHConfig
{
    constexpr explicit BVHConfig(unsigned int max_triangle_in_leaf = 4,
                                 float triangle_intersect_cost = 1.f,
                                 float bbox_intersect_cost = 0.125f) noexcept
        : max_triangles_in_leaf{ max_triangle_in_leaf },
          triangle_intersect_cost{ triangle_intersect_cost },
          bbox_intersect_cost{ bbox_intersect_cost }
    {}

    // Maximum number of triangles in leaf node
    const unsigned int max_triangles_in_leaf;
    // Cost of intersecting a triangle
    const float triangle_intersect_cost;
    // BBox intersect cost
    const float bbox_intersect_cost;
};

class BVH
{
public:
    BVH(const BVHConfig& config, const std::vector<Triangle>& tr);

private:
    const BVHConfig configuration;
    std::vector<Triangle> triangles;
};

} // Geometry namespace

#endif //RABBIT2_BVH_HPP
