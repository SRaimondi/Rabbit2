//
// Created by Simon on 2019-04-01.
//

#ifndef RABBIT2_GEOMETRY_HPP
#define RABBIT2_GEOMETRY_HPP

#include <cmath>
#include <algorithm>
#include <cassert>

namespace Rabbit
{
namespace Geometry
{

template <typename T>
struct Point3
{
    constexpr Point3() noexcept
        : x{ 0 }, y{ 0 }, z{ 0 }
    {}

    constexpr explicit Point3(T v) noexcept
        : x{ v }, y{ v }, z{ v }
    {}

    constexpr Point3(T x, T y, T z)
        : x{ x }, y{ y }, z{ z }
    {}

    constexpr T operator[](unsigned int i) const noexcept
    {
        assert(i < 3);
        if (i == 0)
        {
            return x;
        }
        else if (i == 1)
        {
            return y;
        }
        else
        {
            return z;
        }
    }

    // Point elements
    T x, y, z;
};

template <typename T>
struct Point2
{
    constexpr Point2() noexcept
        : x{ 0 }, y{ 0 }
    {}

    constexpr explicit Point2(T v) noexcept
        : x{ v }, y{ v }
    {}

    constexpr Point2(T x, T y)
        : x{ x }, y{ y }
    {}

    constexpr T operator[](unsigned int i) const noexcept
    {
        assert(i < 2);
        if (i == 0)
        {
            return x;
        }
        else
        {
            return y;
        }
    }

    // Point elements
    T x, y;
};

template <typename T>
struct Vector3
{
    constexpr Vector3() noexcept
        : x{ 0 }, y{ 0 }, z{ 0 }
    {}

    constexpr explicit Vector3(T v) noexcept
        : x{ v }, y{ v }, z{ v }
    {}

    constexpr Vector3(T x, T y, T z) noexcept
        : x{ x }, y{ y }, z{ z }
    {}

    constexpr T operator[](unsigned int i) const noexcept
    {
        assert(i < 3);
        if (i == 0)
        {
            return x;
        }
        else if (i == 1)
        {
            return y;
        }
        else
        {
            return z;
        }
    }

    Vector3& operator+=(const Vector3<T>& v) noexcept
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    constexpr unsigned int LargestDimension() const noexcept
    {
        if (x > y && x > z)
        {
            return 0;
        }
        else if (y > z)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }

    // Vector components
    T x, y, z;
};

template <typename T>
struct Vector2
{
    constexpr Vector2() noexcept
        : x{ 0 }, y{ 0 }
    {}

    constexpr explicit Vector2(T v) noexcept
        : x{ v }, y{ v }
    {}

    constexpr Vector2(T x, T y) noexcept
        : x{ x }, y{ y }
    {}

    constexpr T operator[](unsigned int i) const noexcept
    {
        assert(i < 2);
        if (i == 0)
        {
            return x;
        }
        else
        {
            return y;
        }
    }

    // Vector components
    T x, y;
};

// Math operations
template <typename T>
constexpr const Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs) noexcept
{
    return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

template <typename T>
constexpr const Point3<T> operator+(const Point3<T>& lhs, const Vector3<T>& rhs) noexcept
{
    return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

template <typename T>
constexpr const Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs) noexcept
{
    return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

template <typename T>
constexpr const Vector3<T> operator-(const Point3<T>& lhs, const Point3<T>& rhs) noexcept
{
    return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

template <typename T>
constexpr const Vector3<T> operator-(const Vector3<T>& v) noexcept
{
    return { -v.x, -v.y, -v.z };
}

template <typename T>
constexpr const Vector3<T> operator*(const Vector3<T>& lhs, const Vector3<T>& rhs) noexcept
{
    return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

template <typename T>
constexpr const Vector3<T> operator*(T lhs, const Vector3<T>& rhs) noexcept
{
    return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
}

template <typename T>
constexpr const Vector3<T> operator*(const Vector3<T>& lhs, T rhs) noexcept
{
    return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

template <typename T>
constexpr const Vector3<T> operator/(const Vector3<T>& lhs, const Vector3<T>& rhs) noexcept
{
    return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
}

template <typename T>
constexpr const Vector3<T> operator/(const Vector3<T>& lhs, T rhs) noexcept
{
    const T inv_rhs{ 1.f / rhs };
    return inv_rhs * lhs;
}

template <typename T>
constexpr const Vector3<T> Cross(const Vector3<T>& lhs, const Vector3<T>& rhs) noexcept
{
    return { lhs.y * rhs.z - lhs.z * rhs.y,
             lhs.z * rhs.x - lhs.x * rhs.z,
             lhs.x * rhs.y - lhs.y * rhs.x };
}

template <typename T>
constexpr T Dot(const Vector3<T>& lhs, const Vector3<T>& rhs) noexcept
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template <typename T>
constexpr T SquaredNorm(const Vector3<T>& v) noexcept
{
    return Dot(v, v);
}

template <typename T>
inline T Norm(const Vector3<T>& v) noexcept
{
    return std::sqrt(SquaredNorm(v));
}

template <typename T>
inline const Vector3<T> Normalize(const Vector3<T>& v) noexcept
{
    return v / Norm(v);
}

template <typename T>
inline void NormalizeInPlace(Vector3<T>& v) noexcept
{
    const T inv_norm{ 1.f / Norm(v) };
    v.x *= inv_norm;
    v.y *= inv_norm;
    v.z *= inv_norm;
}

template <typename T>
constexpr const Point3<T> Max(const Point3<T>& lhs, const Point3<T>& rhs) noexcept
{
    return { std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z) };
}

template <typename T>
constexpr const Vector3<T> Max(const Vector3<T>& lhs, const Vector3<T>& rhs) noexcept
{
    return { std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z) };
}

template <typename T>
constexpr float HorizontalMax(const Vector3<T>& v) noexcept
{
    return std::max(v.x, std::max(v.y, v.z));
}

template <typename T>
constexpr const Point3<T> Min(const Point3<T>& lhs, const Point3<T>& rhs) noexcept
{
    return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z) };
}

template <typename T>
constexpr const Vector3<T> Min(const Vector3<T>& lhs, const Vector3<T>& rhs) noexcept
{
    return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z) };
}

template <typename T>
constexpr T HorizontalMin(const Vector3<T>& v) noexcept
{
    return std::min(v.x, std::min(v.y, v.z));
}

template <typename T>
inline const Vector3<T> Abs(const Vector3<T>& v) noexcept
{
    return { std::abs(v.x), std::abs(v.y), std::abs(v.z) };
}

template <typename T>
constexpr const Vector3<T> Permute(const Vector3<T>& v, unsigned int kx, unsigned int ky, unsigned int kz) noexcept
{
    return { v[kx], v[ky], v[kz] };
}

template <typename T>
inline T Distance(const Point3<T>& from, const Point3<T>& to) noexcept
{
    return Norm(to - from);
}

// Reciprocal 1 / v
template <typename T>
constexpr const Vector3<T> Reciprocal(const Vector3<T>& v) noexcept
{
    return { 1.f / v.x, 1.f / v.y, 1.f / v.z };
}

using Point3f = Point3<float>;
using Point2f = Point2<float>;
using Point2ui = Point2<unsigned int>;
using Vector3f = Vector3<float>;
using Vector2f = Vector2<float>;

} // Geometry namespace
} // Rabbit namespace

#endif //RABBIT2_GEOMETRY_HPP
