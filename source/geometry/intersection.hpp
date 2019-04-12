//
// Created by Simon on 2019-03-28.
//

#ifndef RABBIT2_INTERSECTION_HPP
#define RABBIT2_INTERSECTION_HPP

#include "frame.hpp"
#include "ray.hpp"

namespace Rabbit
{

// Forward declare triangle class
class Triangle;

namespace Geometry
{

struct TriangleIntersection
{
    TriangleIntersection() noexcept
        : hit_triangle{ nullptr }
    {}

    // Check if intersection represents and hit
    bool IsValid() const noexcept
    {
        return hit_triangle != nullptr;
    }

    Ray SpawnRay(const Vector3f& direction) const noexcept
    {
        return { hit_point + EPS<float> * local_geometry.n, direction };
    }

    // Hit point
    Point3f hit_point;
    // Local base at hit point
    Framef local_geometry;
    // Outgoing light direction at intersection
    Vector3f wo;
    // UV coordinates
    Vector2f uv;
    // Barycentric coordinates
    Point3f barycentric_coordinates;
    // Pointer to triangle that generated the intersection
    const Triangle* hit_triangle;
};

} // Geometry namespace
} // Rabbit namespace

#endif //RABBIT2_INTERSECTION_HPP
