//
// Created by Simon on 2019-03-28.
//

#ifndef RABBIT2_INTERSECTION_HPP
#define RABBIT2_INTERSECTION_HPP

#include "geometry.hpp"

namespace Rabbit
{
namespace Geometry
{

struct TriangleIntersection
{
    // Index for triangle to indicate no hit
    static constexpr uint32_t INVALID_TRIANGLE_IDX{ std::numeric_limits<uint32_t>::max() };

    TriangleIntersection() noexcept
        : u{ 0.f }, v{ 0.f }, w{ 0.f }, triangle_index{ INVALID_TRIANGLE_IDX }
    {}

    // Check if intersection represents and hit
    bool IsValid() const noexcept
    {
        return triangle_index != INVALID_TRIANGLE_IDX;
    }

    // Hit point
    Point3f hit_point;
    // Barycentric coordinate for vertex v0
    float u;
    // Normal at hit point
    Vector3f normal;
    // Barycentric coordinate for vertex v1
    float v;
    // UV coordinates
    Point2f uv;
    // Barycentric coordinate for vertex v2
    float w;
    // Index of the triangle that generated the intersection
    uint32_t triangle_index;
};

} // Geometry namespace
} // Rabbit namespace

#endif //RABBIT2_INTERSECTION_HPP
