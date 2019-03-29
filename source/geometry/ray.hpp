//
// Created by Simon on 2019-03-28.
//

#ifndef RABBIT2_RAY_HPP
#define RABBIT2_RAY_HPP

#include "vector.hpp"

namespace Geometry
{

// Ray class
struct alignas(32) Ray
{
    constexpr Ray() noexcept
        : extent_start{ DEFAULT_EXTENT_START }, extent_end{ DEFAULT_EXTENT_END }
    {}

    constexpr Ray(const Vector3& o, const Vector3& d,
                  float start = DEFAULT_EXTENT_START, float end = DEFAULT_EXTENT_END) noexcept
        : origin{ o }, extent_start{ start }, direction{ d }, extent_end{ end }
    {}

    constexpr bool IsInRange(float t) const noexcept
    {
        return (t >= extent_start) && (t <= extent_end);
    }

    constexpr const Vector3 operator()(float t) const noexcept
    {
        return origin + t * direction;
    }

    static constexpr float DEFAULT_EXTENT_START{ 0.001f };
    static constexpr float DEFAULT_EXTENT_END{ std::numeric_limits<float>::max() };

    // Ray description
    Vector3 origin;
    float extent_start;
    Vector3 direction;
    float extent_end;
};

} // Geometry namespace

#endif //RABBIT2_RAY_HPP
