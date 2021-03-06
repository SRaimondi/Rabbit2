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
        : Point3{ 0 }
    {}

    constexpr explicit Point3(T v) noexcept
        : Point3{ v, v, v }
    {}

    constexpr Point3(T x, T y, T z)
        : x{ x }, y{ y }, z{ z }
    {}

    constexpr T operator[](unsigned int i) const noexcept
    {
        assert(i < 3);
        return (&x)[i];
    }

    // Point elements
    T x, y, z;
};

template <typename T>
struct Point2
{
    constexpr Point2() noexcept
        : Point2{ 0 }
    {}

    constexpr explicit Point2(T v) noexcept
        : Point2{ v, v }
    {}

    constexpr Point2(T x, T y)
        : x{ x }, y{ y }
    {}

    constexpr T operator[](unsigned int i) const noexcept
    {
        assert(i < 2);
        return (&x)[i];
    }

    // Point elements
    T x, y;
};

template <typename T>
struct Vector3
{
    constexpr Vector3() noexcept
        : Vector3{ 0 }
    {}

    constexpr explicit Vector3(T v) noexcept
        : Vector3{ v, v, v }
    {}

    constexpr Vector3(T x, T y, T z) noexcept
        : x{ x }, y{ y }, z{ z }
    {}

    constexpr T operator[](unsigned int i) const noexcept
    {
        assert(i < 3);
        return (&x)[i];
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
        : Vector2{ 0 }
    {}

    constexpr explicit Vector2(T v) noexcept
        : Vector2{ v, v }
    {}

    constexpr Vector2(T x, T y) noexcept
        : x{ x }, y{ y }
    {}

    constexpr T operator[](unsigned int i) const noexcept
    {
        assert(i < 2);
        return (&x)[i];
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
constexpr const Point3<T> operator+(const Point3<T>& lhs, const Point3<T>& rhs) noexcept
{
    return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

template <typename T>
constexpr const Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs) noexcept
{
    return { lhs.x + rhs.x, lhs.y + rhs.y };
}

template <typename T>
constexpr const Point3<T> operator+(const Point3<T>& lhs, const Vector3<T>& rhs) noexcept
{
    return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

template <typename T>
constexpr const Point3<T> operator-(const Point3<T>& lhs, const Vector3<T>& rhs) noexcept
{
    return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

template <typename T>
constexpr const Point2<T> operator-(const Point2<T>& lhs, const Vector2<T>& rhs) noexcept
{
    return { lhs.x - rhs.x, lhs.y - rhs.y };
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
constexpr const Point3<T> operator*(T lhs, const Point3<T>& rhs) noexcept
{
    return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
}

template <typename T>
constexpr const Vector2<T> operator*(T lhs, const Vector2<T>& rhs) noexcept
{
    return { lhs * rhs.x, lhs * rhs.y };
}

template <typename T>
constexpr const Point2<T> operator*(T lhs, const Point2<T>& rhs) noexcept
{
    return { lhs * rhs.x, lhs * rhs.y };
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
    const T inv_rhs{ 1 / rhs };
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
constexpr T AbsDot(const Vector3<T>& lhs, const Vector3<T>& rhs) noexcept
{
    return std::abs(Dot(lhs, rhs));
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
constexpr T DistanceSquared(const Point3<T>& from, const Point3<T>& to) noexcept
{
    return SquaredNorm(to - from);
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
    return { 1 / v.x, 1 / v.y, 1 / v.z };
}

// Create local base around given vector
template <typename T>
inline void CreateLocalBase(const Vector3<T>& n, Vector3<T>& s, Vector3<T>& t) noexcept
{
    if (std::abs(n.x) > std::abs(n.y))
    {
        s = Vector3<T>{ -n.z, T(0), n.x } / std::sqrt(n.x * n.x + n.z * n.z);
    }
    else
    {
        s = Vector3<T>{ T(0), n.z, -n.y } / std::sqrt(n.y * n.y + n.z * n.z);
    }
    t = Cross(s, n);
}

// Reflect vector along normal
template <typename T>
constexpr Vector3<T> Reflect(const Vector3<T>& v, const Vector3<T>& n) noexcept
{
    return 2.f * Dot(v, n) * n - v;
}

using Point3f = Point3<float>;
using Point2f = Point2<float>;
using Point2ui = Point2<unsigned int>;
using Vector3f = Vector3<float>;
using Vector2f = Vector2<float>;

} // Geometry namespace
} // Rabbit namespace

#endif //RABBIT2_GEOMETRY_HPP
