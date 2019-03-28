//
// Created by Simon on 2019-03-28.
//

#ifndef RABBIT2_RAY_HPP
#define RABBIT2_RAY_HPP

#include "vector.hpp"

namespace Geometry
{

// Ray class
struct Ray
{
    constexpr Ray() noexcept
        : extent{ DEFAULT_EXTENT }
    {}

    constexpr Ray(const Vector3& o, const Vector3& d, float ext = DEFAULT_EXTENT) noexcept
        : origin{ o }, direction{ d }, extent{ ext }
    {}

    constexpr const Vector3 operator()(float t) const noexcept
    {
        return origin + t * direction;
    }

    static constexpr float DEFAULT_EXTENT{ std::numeric_limits<float>::max() };

    // Ray origin, direction and extent
    Vector3 origin, direction;
    float extent;
};

} // Geometry namespace

#endif //RABBIT2_RAY_HPP
