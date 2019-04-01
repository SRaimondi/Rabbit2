//
// Created by Simon on 2019-03-28.
//

#ifndef RABBIT2_RAY_HPP
#define RABBIT2_RAY_HPP

#include "geometry.hpp"

#include <limits>

namespace Geometry
{

// Ray class
struct alignas(32) Ray
{
    constexpr Ray() noexcept
        : extent_start{ DEFAULT_EXTENT_START }, extent_end{ DEFAULT_EXTENT_END }
    {}

    constexpr Ray(const Point3& o, const Vector3& d,
                  float start = DEFAULT_EXTENT_START, float end = DEFAULT_EXTENT_END) noexcept
        : origin{ o }, extent_start{ start }, direction{ d }, extent_end{ end }
    {}

    constexpr const Point3 operator()(float t) const noexcept
    {
        return origin + t * direction;
    }

    static constexpr float DEFAULT_EXTENT_START{ 0.001f };
    static constexpr float DEFAULT_EXTENT_END{ std::numeric_limits<float>::max() };

    // Ray description
    Point3 origin;
    float extent_start;
    Vector3 direction;
    float extent_end;
};

} // Geometry namespace

#endif //RABBIT2_RAY_HPP
