//
// Created by Simon on 2019-03-27.
//

#ifndef RABBIT2_BBOX_HPP
#define RABBIT2_BBOX_HPP

#include "vector.hpp"

#include <array>

namespace Geometry
{

class BBox
{
public:
    constexpr BBox() noexcept
        : bounds{ Vector3{ std::numeric_limits<float>::max() },
                  Vector3{ std::numeric_limits<float>::lowest() }}
    {}

    constexpr BBox(const Vector3& min, const Vector3& max) noexcept
        : bounds{ min, max }
    {}

    constexpr BBox(std::initializer_list<Vector3> points) noexcept
        : bounds{ Min(points), Max(points) }
    {}

    constexpr const Vector3& operator[](int i) const noexcept
    {
        assert(i < 2);
        return bounds[i];
    }

    constexpr const Vector3 Diagonal() const noexcept
    {
        return bounds[1] - bounds[0];
    }

    constexpr float Surface() const noexcept
    {
        const Vector3 diagonal{ Diagonal() };
        return 2.f * (diagonal.x * diagonal.y + diagonal.x * diagonal.z + diagonal.y * diagonal.z);
    }

private:
    // Bounds
    std::array<Vector3, 2> bounds;
};

constexpr const BBox Union(const BBox& bbox, const Vector3& v) noexcept
{
    return { Min(bbox[0], v), Max(bbox[1], v) };
}

constexpr const BBox Union(const BBox& bbox1, const BBox& bbox2) noexcept
{
    return { Min(bbox1[0], bbox2[0]), Max(bbox1[1], bbox2[1]) };
}

} // Geometry namespace

#endif //RABBIT2_BBOX_HPP
