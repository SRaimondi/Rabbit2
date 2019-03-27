//
// Created by Simon on 2019-03-26.
//

#ifndef RABBIT2_VECTOR_HPP
#define RABBIT2_VECTOR_HPP

#include <cmath>
#include <algorithm>

namespace Geometry
{

// Minimal 3d vector
struct Vector3
{
    // Vector components
    float x, y, z;

    constexpr Vector3() noexcept
        : x{ 0.f }, y{ 0.f }, z{ 0.f }
    {}

    constexpr explicit Vector3(float v) noexcept
        : x{ v }, y{ v }, z{ v }
    {}

    constexpr Vector3(float x, float y, float z) noexcept
        : x{ x }, y{ y }, z{ z }
    {}

    Vector3& operator+=(const Vector3& v) noexcept
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
};

// Math operations
constexpr const Vector3 operator+(const Vector3& lhs, const Vector3& rhs) noexcept
{
    return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

constexpr const Vector3 operator-(const Vector3& lhs, const Vector3& rhs) noexcept
{
    return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

constexpr const Vector3 operator-(const Vector3& v) noexcept
{
    return { -v.x, -v.y, -v.z };
}

constexpr const Vector3 operator*(float lhs, const Vector3& rhs) noexcept
{
    return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
}

constexpr const Vector3 operator*(const Vector3& lhs, float rhs) noexcept
{
    return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

constexpr const Vector3 operator/(const Vector3& lhs, float rhs) noexcept
{
    const float inv_rhs{ 1.f / rhs };
    return inv_rhs * lhs;
}

constexpr const Vector3 Cross(const Vector3& lhs, const Vector3& rhs) noexcept
{
    return { lhs.y * rhs.z - lhs.z * rhs.y,
             lhs.z * rhs.x - lhs.x * rhs.z,
             lhs.x * rhs.y - lhs.y * rhs.x };
}

constexpr float Dot(const Vector3& lhs, const Vector3& rhs) noexcept
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

constexpr float SquaredNorm(const Vector3& v) noexcept
{
    return Dot(v, v);
}

inline float Norm(const Vector3& v) noexcept
{
    return std::sqrt(SquaredNorm(v));
}

inline const Vector3 Normalize(const Vector3& v) noexcept
{
    return v / Norm(v);
}

inline void NormalizeInPlace(Vector3& v) noexcept
{
    const float inv_norm{ 1.f / Norm(v) };
    v.x *= inv_norm;
    v.y *= inv_norm;
    v.z *= inv_norm;
}

constexpr const Vector3 Max(const Vector3& lhs, const Vector3& rhs) noexcept
{
    return { std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z) };
}

constexpr const Vector3 Max(std::initializer_list<Vector3> vectors) noexcept
{
    Vector3 max{ std::numeric_limits<float>::lowest() };
    for (const auto& vector : vectors)
    {
        max = Max(max, vector);
    }

    return max;
}

constexpr const Vector3 Min(const Vector3& lhs, const Vector3& rhs) noexcept
{
    return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z) };
}

constexpr const Vector3 Min(std::initializer_list<Vector3> vectors) noexcept
{
    Vector3 min{ std::numeric_limits<float>::max() };
    for (const auto& vector : vectors)
    {
        min = Min(min, vector);
    }

    return min;
}

} // Geometry namespace

#endif //RABBIT2_VECTOR_HPP