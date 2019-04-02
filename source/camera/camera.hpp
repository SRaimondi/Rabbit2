//
// Created by Simon on 2019-03-29.
//

#ifndef RABBIT2_CAMERA_HPP
#define RABBIT2_CAMERA_HPP

#include "geometry/ray.hpp"

class Camera
{
public:
    Camera(const Geometry::Point3f& eye, const Geometry::Point3f& at, const Geometry::Vector3f& up,
           float fov, unsigned int image_width, unsigned int image_height) noexcept;

    // Generate ray at given coordinates and offset
    const Geometry::Ray GenerateRay(const Geometry::Point2ui& pixel_coordinates,
                                    const Geometry::Point2f& sample_offset) const noexcept;

private:
    // Eye position
    const Geometry::Point3f eye;
    // Local base
    Geometry::Vector3f u, v, w;
    // View volume
    float left, right, top, bottom;
    // Inverse size of the image
    const float inv_width, inv_height;
};

#endif //RABBIT2_CAMERA_HPP
