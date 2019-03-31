//
// Created by Simon on 2019-03-27.
//

#ifndef RABBIT2_BBOX_HPP
#define RABBIT2_BBOX_HPP

#include "ray.hpp"

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

    constexpr BBox(const Vector3& v0, const Vector3& v1, const Vector3& v2) noexcept
        : bounds{ Min(v0, Min(v1, v2)), Max(v0, Max(v1, v2)) }
    {}

    constexpr const Vector3& operator[](unsigned int i) const noexcept
    {
        assert(i < 2);
        return bounds[i];
    }

    constexpr const Vector3& PMin() const noexcept
    {
        return bounds[0];
    }

    constexpr const Vector3& PMax() const noexcept
    {
        return bounds[1];
    }

    constexpr const Vector3 Diagonal() const noexcept
    {
        return PMax() - PMin();
    }

    constexpr float Surface() const noexcept
    {
        const Vector3 diagonal{ Diagonal() };
        return 2.f * (diagonal.x * diagonal.y + diagonal.x * diagonal.z + diagonal.y * diagonal.z);
    }

    constexpr float Volume() const noexcept
    {
        const Vector3 diagonal{ Diagonal() };
        return diagonal.x * diagonal.y * diagonal.z;
    }

    constexpr const Vector3 Centroid() const noexcept
    {
        return 0.5f * (PMin() + PMax());
    }

    constexpr unsigned int LargestDimension() const noexcept
    {
        return Diagonal().LargestDimension();
    }

    // Compute offset of a point in the BBox
    constexpr const Vector3 Offset(const Vector3& v) const noexcept
    {
        return (v - PMin()) / Diagonal();
    }

    // Check for intersection between a ray and the BBox
    constexpr bool Intersect(const Ray& ray,
                             const Vector3& inv_dir) const noexcept
    {
        // Compute intersection of ray with the bounds slabs
        const Vector3 bounds_min{ (PMin() - ray.origin) * inv_dir };
        const Vector3 bounds_max{ (PMax() - ray.origin) * inv_dir };
        const Vector3 slabs_min{ Min(bounds_min, bounds_max) };
        const Vector3 slabs_max{ Max(bounds_min, bounds_max) };

        // Select minimum and maximum
        const float t_min{ HorizontalMax(slabs_min) };
        const float t_max{ HorizontalMin(slabs_max) };

        // Check if we did hit
        return (t_min < t_max) && (t_min < ray.extent_end) && (t_max > ray.extent_start);
    }

private:
    // Bounds
    Vector3 bounds[2];
};

constexpr const BBox Union(const BBox& bbox, const Vector3& v) noexcept
{
    return { Min(bbox.PMin(), v), Max(bbox.PMax(), v) };
}

constexpr const BBox Union(const BBox& bbox1, const BBox& bbox2) noexcept
{
    return { Min(bbox1.PMin(), bbox2.PMin()), Max(bbox1.PMax(), bbox2.PMax()) };
}

constexpr bool Overlap(const BBox& bbox1, const BBox& bbox2) noexcept
{
    const bool overlap_x{ (bbox1.PMax().x >= bbox2.PMin().x) && (bbox1.PMin().x <= bbox2.PMax().x) };
    const bool overlap_y{ (bbox1.PMax().y >= bbox2.PMin().y) && (bbox1.PMin().y <= bbox2.PMax().y) };
    const bool overlap_z{ (bbox1.PMax().z >= bbox2.PMin().z) && (bbox1.PMin().z <= bbox2.PMax().z) };

    return overlap_x && overlap_y && overlap_z;
}

constexpr const BBox Intersection(const BBox& bbox1, const BBox& bbox2) noexcept
{
    return { Max(bbox1.PMin(), bbox2.PMin()), Min(bbox1.PMax(), bbox2.PMax()) };
}

} // Geometry namespace

#endif //RABBIT2_BBOX_HPP
