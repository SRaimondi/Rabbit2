//
// Created by Simon on 2019-03-29.
//

#include "camera.hpp"
#include "geometry/common.hpp"

Camera::Camera(const Geometry::Point3f& eye, const Geometry::Point3f& at, const Geometry::Vector3f& up, float fov,
               unsigned int image_width, unsigned int image_height) noexcept
    : eye{ eye }, inv_width{ 1.f / image_width }, inv_height{ 1.f / image_height }
{
    using namespace Geometry;

    // Compute local base
    w = Normalize(eye - at);
    u = Normalize(Cross(up, w));
    v = Cross(w, u);

    // Compute view volume
    top = std::tan(Radians(0.5f * fov));
    bottom = -top;
    right = top * image_width / image_height;
    left = -right;
}

const Geometry::Ray Camera::GenerateRay(const Geometry::Point2ui& pixel_coordinates,
                                        const Geometry::Point2f& sample_offset) const noexcept
{
    // Compute pixel coordinates on view plane
    const float view_plane_x{ left + (right - left) * (pixel_coordinates.x + sample_offset.x) * inv_width };
    const float view_plane_y{ bottom + (top - bottom) * (pixel_coordinates.y + sample_offset.y) * inv_height };

    return { eye, Geometry::Normalize(view_plane_x * u + view_plane_y * v - w) };
}
