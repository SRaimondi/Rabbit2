//
// Created by Simon on 2019-04-11.
//

#ifndef RABBIT2_FRAME_HPP
#define RABBIT2_FRAME_HPP

#include "geometry.hpp"

namespace Rabbit
{
namespace Geometry
{

template <typename T>
struct Frame
{
public:
    constexpr Frame() noexcept = default;

    explicit Frame(const Vector3 <T>& n) noexcept
        : n{ n }
    {
        CreateLocalBase(n, s, t);
    }

    Frame(const Vector3 <T>& s, const Vector3 <T>& n, const Vector3 <T>& t) noexcept
        : s{ s }, n{ n }, t{ t }
    {}

    // Transform direction to local space
    const Vector3 <T> ToLocal(const Vector3 <T>& dir) const noexcept
    {
        return { Dot(dir, s), Dot(dir, n), Dot(dir, t) };
    }

    // Transform direction to world space
    const Vector3 <T> ToWorld(const Vector3 <T>& dir) const noexcept
    {
        return dir.x * s + dir.y * n + dir.z * t;
    }

    // Static utilities to compute values when in local space
    static constexpr T CosTheta(const Vector3 <T>& dir) noexcept
    {
        return dir.y;
    }

    // Local base
    Vector3 <T> s, n, t;
};

using Framef = Frame<float>;

} // Geometry namespace
} // Rabbit namespace

#endif //RABBIT2_FRAME_HPP
