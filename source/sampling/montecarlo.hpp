//
// Created by Simon on 2019-04-09.
//

#ifndef RABBIT2_MONTECARLO_HPP
#define RABBIT2_MONTECARLO_HPP

#include <geometry/common.hpp>
#include "geometry/geometry.hpp"

namespace Rabbit
{
namespace Sampling
{

inline Geometry::Vector3f UniformSampleHemisphere(const Geometry::Point2f& u) noexcept
{
    const float y{ u[0] };
    const float radius{ std::sqrt(std::max(0.f, 1.f - y * y)) };
    const float phi{ Geometry::TWO_PI<float> * u[1] };

    return { radius * std::cos(phi), y, radius * std::sin(phi) };
}

constexpr float UniformSampleHemispherePdf() noexcept
{
    return Geometry::INV_2PI<float>;
}

inline Geometry::Vector3f UniformSampleSphere(const Geometry::Point2f& u) noexcept
{
    const float y{ 1.f - 2.f * u[0] };
    const float radius{ std::sqrt(std::max(0.f, 1.f - y * y)) };
    const float phi{ Geometry::TWO_PI<float> * u[1] };

    return { radius * std::cos(phi), y, radius * std::sin(phi) };
}

constexpr float UniformSampleSpherePdf() noexcept
{
    return Geometry::INV_4PI<float>;
}

inline Geometry::Point2f ConcentricSampleDisk(const Geometry::Point2f& u) noexcept
{
    // Map uniform value to [-1,1]
    const Geometry::Point2f u_mapped{ 2.f * u - Geometry::Vector2f{ 1.f }};
    // Handle case when we are at the origin
    if (u_mapped.x == 0.f && u_mapped.y == 0.f)
    {
        return Geometry::Point2f{ 0.f };
    }

    float theta, r;
    if (std::abs(u_mapped.x) > std::abs(u_mapped.y))
    {
        r = u_mapped.x;
        theta = Geometry::PI_OVER_4<float> * (u_mapped.y / u_mapped.x);
    }
    else
    {
        r = u_mapped.y;
        theta = Geometry::PI_OVER_2<float> - Geometry::PI_OVER_4<float> * (u_mapped.x / u_mapped.y);
    }

    return r * Geometry::Point2f{ std::cos(theta), std::sin(theta) };
}

inline Geometry::Vector3f CosineSampleHemisphere(const Geometry::Point2f& u) noexcept
{
    const Geometry::Point2f p_circle{ ConcentricSampleDisk(u) };
    const float y{ std::sqrt(std::max(0.f, 1.f - p_circle.x * p_circle.x - p_circle.y * p_circle.y)) };

    return { p_circle.x, y, p_circle.y };
}

constexpr float CosineSampleHemispherePdf(float cos_theta) noexcept
{
    return cos_theta * Geometry::INV_PI<float>;
}

} // Sampling namespace
} // Rabbit namespace

#endif //RABBIT2_MONTECARLO_HPP
