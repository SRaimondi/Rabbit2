//
// Created by simon on 10/04/2019.
//

#include "perspective_camera.hpp"

namespace Rabbit
{

PerspectiveCamera::PerspectiveCamera(const Geometry::Point3f& eye, const Geometry::Point3f& at,
                                     const Geometry::Vector3f& up, float fov, unsigned int image_width,
                                     unsigned int image_height) noexcept
    : look_at{ Geometry::LookAt(eye, at, up) }, inv_width{ 1.f / image_width }, inv_height{ 1.f / image_height }
{
    // Compute view volume
    top = std::tan(Geometry::Radians(0.5f * fov));
    bottom = -top;
    right = top * image_width / image_height;
    left = -right;
}

const Geometry::Ray PerspectiveCamera::GenerateRayWorldSpace(const Geometry::Point2ui& pixel_coordinates,
                                                             const Geometry::Point2f& sample_offset) const noexcept
{
    const Geometry::Ray local_ray{ GenerateRayLocalSpace(pixel_coordinates, sample_offset) };

    return { look_at(local_ray.Origin()), look_at(local_ray.Direction()) };
}

const Geometry::Ray PerspectiveCamera::GenerateRayLocalSpace(const Geometry::Point2ui& pixel_coordinates,
                                                             const Geometry::Point2f& sample_offset) const noexcept
{
    const Geometry::Vector3f direction{ left + (right - left) * (pixel_coordinates.x + sample_offset.x) * inv_width,
                                        bottom + (top - bottom) * (pixel_coordinates.y + sample_offset.y) * inv_height,
                                        -1.f };

    return { Geometry::Point3f{}, Geometry::Normalize(direction) };
}
} // Rabbit namespace