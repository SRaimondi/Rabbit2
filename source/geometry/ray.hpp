//
// Created by Simon on 2019-03-28.
//

#ifndef RABBIT2_RAY_HPP
#define RABBIT2_RAY_HPP

#include "geometry.hpp"
#include "interval.hpp"

namespace Rabbit
{
namespace Geometry
{

// Ray class
struct alignas(32) Ray
{
    constexpr Ray() noexcept = default;

    constexpr Ray(const Point3f& o, const Vector3f& d) noexcept
        : origin{ o }, direction{ d }
    {}

    constexpr static Intervalf DefaultInterval() noexcept
    {
        constexpr float DEFAULT_START{ 0.0001f };
        constexpr float DEFAULT_END{ std::numeric_limits<float>::max() };

        return Intervalf{ DEFAULT_START, DEFAULT_END };
    }

    constexpr const Point3f operator()(float t) const noexcept
    {
        return origin + t * direction;
    }

    // Ray description
    Point3f origin;
    Vector3f direction;
};

} // Geometry namespace
} // Rabbit namespace

#endif //RABBIT2_RAY_HPP
