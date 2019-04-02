//
// Created by Simon on 2019-03-29.
//

#include "camera.hpp"
#include "geometry/common.hpp"

Camera::Camera(const Geometry::Point3f& eye, const Geometry::Point3f& at, const Geometry::Vector3f& up, float fov,
               unsigned int image_width, unsigned int image_height) noexcept
    : eye{ eye }, orientation{ 1.f }, inv_width{ 1.f / image_width }, inv_height{ 1.f / image_height }
{
    SetupOrientation(at, up);
    // Compute view volume
    top = std::tan(Geometry::Radians(0.5f * fov));
    bottom = -top;
    right = top * image_width / image_height;
    left = -right;
}

const Geometry::Ray Camera::GenerateRayWorldSpace(const Geometry::Point2ui& pixel_coordinates,
                                                  const Geometry::Point2f& sample_offset) const noexcept
{
    // Compute pixel coordinates on view plane
    const Geometry::Vector3f viewplane_p{
        left + (right - left) * (pixel_coordinates.x + sample_offset.x) * inv_width,
        bottom + (top - bottom) * (pixel_coordinates.y + sample_offset.y) * inv_height,
        -1.f };

    return { eye, Geometry::Normalize(orientation(viewplane_p)) };
}

const Geometry::Ray Camera::GenerateRayLocalSpace(const Geometry::Point2ui& pixel_coordinates,
                                                  const Geometry::Point2f& sample_offset) const noexcept
{
    const Geometry::Vector3f direction{
        left + (right - left) * (pixel_coordinates.x + sample_offset.x) * inv_width,
        bottom + (top - bottom) * (pixel_coordinates.y + sample_offset.y) * inv_height,
        -1.f };

    return { Geometry::Point3f{}, Geometry::Normalize(direction) };
}

void Camera::SetupOrientation(const Geometry::Point3f& at,
                              const Geometry::Vector3f& up) noexcept
{
    using namespace Geometry;
    // Compute local base
    const Vector3f w{ Normalize(eye - at) };
    const Vector3f u{ Normalize(Cross(up, w)) };
    const Vector3f v{ Cross(w, u) };
    // Set matrix entries
    orientation(0, 0) = u.x;
    orientation(1, 0) = u.y;
    orientation(2, 0) = u.z;
    orientation(0, 1) = v.x;
    orientation(1, 1) = v.y;
    orientation(2, 1) = v.z;
    orientation(0, 2) = w.x;
    orientation(1, 2) = w.y;
    orientation(2, 2) = w.z;
}
