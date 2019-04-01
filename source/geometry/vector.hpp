//
// Created by Simon on 2019-03-26.
//

#ifndef RABBIT2_VECTOR_HPP
#define RABBIT2_VECTOR_HPP

#include <cmath>
#include <algorithm>
#include <cassert>

namespace Geometry
{

struct Vector3
{
    constexpr Vector3() noexcept
        : x{ 0.f }, y{ 0.f }, z{ 0.f }
    {}

    constexpr explicit Vector3(float v) noexcept
        : x{ v }, y{ v }, z{ v }
    {}

    constexpr Vector3(float x, float y, float z) noexcept
        : x{ x }, y{ y }, z{ z }
    {}

    constexpr float operator[](unsigned int i) const noexcept
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

    Vector3& operator+=(const Vector3& v) noexcept
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
    float x, y, z;
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

constexpr const Vector3 operator*(const Vector3& lhs, const Vector3& rhs) noexcept
{
    return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

constexpr const Vector3 operator*(float lhs, const Vector3& rhs) noexcept
{
    return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
}

constexpr const Vector3 operator*(const Vector3& lhs, float rhs) noexcept
{
    return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

constexpr const Vector3 operator/(const Vector3& lhs, const Vector3& rhs) noexcept
{
    return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
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

constexpr float HorizontalMax(const Vector3& v) noexcept
{
    return std::max(v.x, std::max(v.y, v.z));
}

constexpr const Vector3 Min(const Vector3& lhs, const Vector3& rhs) noexcept
{
    return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z) };
}

constexpr float HorizontalMin(const Vector3& v) noexcept
{
    return std::min(v.x, std::min(v.y, v.z));
}

inline const Vector3 Abs(const Vector3& v) noexcept
{
    return { std::abs(v.x), std::abs(v.y), std::abs(v.z) };
}

constexpr const Vector3 Permute(const Vector3& v, unsigned int kx, unsigned int ky, unsigned int kz) noexcept
{
    return { v[kx], v[ky], v[kz] };
}

// Reciprocal 1 / v
constexpr const Vector3 Reciprocal(const Vector3& v) noexcept
{
    return { 1.f / v.x, 1.f / v.y, 1.f / v.z };
}

struct Vector2
{
    constexpr Vector2() noexcept
        : x{ 0.f }, y{ 0.f }
    {}

    constexpr explicit Vector2(float v) noexcept
        : x{ v }, y{ v }
    {}

    constexpr Vector2(float x, float y) noexcept
        : x{ x }, y{ y }
    {}

    constexpr float operator[](unsigned int i) const noexcept
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

    Vector2& operator+=(const Vector2& v) noexcept
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    constexpr unsigned int LargestDimension() const noexcept
    {
        if (x > y)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    // Vector components
    float x, y;
};

// Math operations
constexpr const Vector2 operator+(const Vector2& lhs, const Vector2& rhs) noexcept
{
    return { lhs.x + rhs.x, lhs.y + rhs.y };
}

constexpr const Vector2 operator-(const Vector2& lhs, const Vector2& rhs) noexcept
{
    return { lhs.x - rhs.x, lhs.y - rhs.y };
}

constexpr const Vector2 operator-(const Vector2& v) noexcept
{
    return { -v.x, -v.y };
}

constexpr const Vector2 operator*(const Vector2& lhs, const Vector2& rhs) noexcept
{
    return { lhs.x * rhs.x, lhs.y * rhs.y };
}

constexpr const Vector2 operator*(float lhs, const Vector2& rhs) noexcept
{
    return { lhs * rhs.x, lhs * rhs.y };
}

constexpr const Vector2 operator*(const Vector2& lhs, float rhs) noexcept
{
    return { lhs.x * rhs, lhs.y * rhs };
}

constexpr const Vector2 operator/(const Vector2& lhs, const Vector2& rhs) noexcept
{
    return { lhs.x / rhs.x, lhs.y / rhs.y };
}

constexpr const Vector2 operator/(const Vector2& lhs, float rhs) noexcept
{
    const float inv_rhs{ 1.f / rhs };
    return inv_rhs * lhs;
}

constexpr float Dot(const Vector2& lhs, const Vector2& rhs) noexcept
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

constexpr float SquaredNorm(const Vector2& v) noexcept
{
    return Dot(v, v);
}

inline float Norm(const Vector2& v) noexcept
{
    return std::sqrt(SquaredNorm(v));
}

inline const Vector2 Normalize(const Vector2& v) noexcept
{
    return v / Norm(v);
}

inline void NormalizeInPlace(Vector2& v) noexcept
{
    const float inv_norm{ 1.f / Norm(v) };
    v.x *= inv_norm;
    v.y *= inv_norm;
}

constexpr const Vector2 Max(const Vector2& lhs, const Vector2& rhs) noexcept
{
    return { std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y) };
}

constexpr float HorizontalMax(const Vector2& v) noexcept
{
    return std::max(v.x, v.y);
}

constexpr const Vector2 Min(const Vector2& lhs, const Vector2& rhs) noexcept
{
    return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y) };
}

constexpr float HorizontalMin(const Vector2& v) noexcept
{
    return std::min(v.x, v.y);
}

inline const Vector2 Abs(const Vector2& v) noexcept
{
    return { std::abs(v.x), std::abs(v.y) };
}

constexpr const Vector2 Permute(const Vector2& v, unsigned int kx, unsigned int ky) noexcept
{
    return { v[kx], v[ky] };
}

// Reciprocal 1 / v
constexpr const Vector2 Reciprocal(const Vector2& v) noexcept
{
    return { 1.f / v.x, 1.f / v.y };
}

} // Geometry namespace

#endif //RABBIT2_VECTOR_HPP
