//
// Created by Simon on 2019-03-29.
//

#include "camera.hpp"
#include "geometry/common.hpp"

Camera::Camera(const Geometry::Vector3& eye, const Geometry::Vector3& at, const Geometry::Vector3& up, float fov,
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

const Geometry::Ray Camera::GenerateRay(unsigned int pixel_x, unsigned int pixel_y,
                                        float offset_x, float offset_y) const noexcept
{
    // Compute pixel coordinates on view plane
    const float viewplane_x{ left + (right - left) * (pixel_x + offset_x) * inv_width };
    const float viewplane_y{ bottom + (top - bottom) * (pixel_y + offset_y) * inv_height };

    return { eye, Geometry::Normalize(viewplane_x * u + viewplane_y * v - w) };
}
