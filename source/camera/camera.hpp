//
// Created by Simon on 2019-03-29.
//

#ifndef RABBIT2_CAMERA_HPP
#define RABBIT2_CAMERA_HPP

#include "geometry/matrix.hpp"
#include "geometry/ray.hpp"

namespace Rabbit
{

class CameraInterface
{
public:
    // Generate ray at given coordinates and offset in world space
    virtual const Geometry::Ray GenerateRayWorldSpace(const Geometry::Point2ui& pixel_coordinates,
                                                      const Geometry::Point2f& sample_offset) const noexcept = 0;

    // Generate ray in camera space
    virtual const Geometry::Ray GenerateRayLocalSpace(const Geometry::Point2ui& pixel_coordinates,
                                                      const Geometry::Point2f& sample_offset) const noexcept = 0;
};

} // Rabbit namespace

#endif //RABBIT2_CAMERA_HPP
