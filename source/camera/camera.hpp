//
// Created by Simon on 2019-03-29.
//

#ifndef RABBIT2_CAMERA_HPP
#define RABBIT2_CAMERA_HPP

#include "geometry/ray.hpp"

class Camera
{
public:
    Camera(const Geometry::Vector3& eye, const Geometry::Vector3& at, const Geometry::Vector3& up,
           float fov, unsigned int image_width, unsigned int image_height) noexcept;

    // Generate ray at given coordinates and offset
    const Geometry::Ray GenerateRay(unsigned int pixel_x, unsigned int pixel_y,
                                    float offset_x, float offset_y) const noexcept;

private:
    // Eye position
    const Geometry::Vector3 eye;
    // Local base
    Geometry::Vector3 u, v, w;
    // View volume
    float left, right, top, bottom;
    // Inverse size of the image
    const float inv_width, inv_height;
};

#endif //RABBIT2_CAMERA_HPP
