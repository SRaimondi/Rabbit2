//
// Created by Simon on 2019-03-28.
//

#ifndef RABBIT2_INTERSECTION_HPP
#define RABBIT2_INTERSECTION_HPP

#include "geometry.hpp"

namespace Rabbit
{

// Forward declare triangle class
class Triangle;

namespace Geometry
{

struct TriangleIntersection
{
    TriangleIntersection() noexcept
        : u{ 0.f }, v{ 0.f }, w{ 0.f }, hit_triangle{ nullptr }
    {}

    // Check if intersection represents and hit
    bool IsValid() const noexcept
    {
        return hit_triangle != nullptr;
    }

    // Hit point
    Point3f hit_point;
    // Normal at hit point + local frame
    Vector3f normal;
    Vector3f s, t;
    // Outgoing light direction at intersection
    Vector3f wo;
    // UV coordinates
    Vector2f uv;
    // Barycentric coordinates
    float u, v, w;
    // Pointer to triangle that generated the intersection
    const Triangle* hit_triangle;
};

} // Geometry namespace
} // Rabbit namespace

#endif //RABBIT2_INTERSECTION_HPP
