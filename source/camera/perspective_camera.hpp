//
// Created by simon on 10/04/2019.
//

#ifndef RABBIT2_PERSPECTIVE_CAMERA_HPP
#define RABBIT2_PERSPECTIVE_CAMERA_HPP

#include "camera.hpp"

namespace Rabbit
{

class PerspectiveCamera : public CameraInterface
{
public:
    PerspectiveCamera(const Geometry::Point3f& eye, const Geometry::Point3f& at, const Geometry::Vector3f& up,
                      float fov, unsigned int image_width, unsigned int image_height) noexcept;

    const Geometry::Ray GenerateRayWorldSpace(const Geometry::Point2ui& pixel_coordinates,
                                              const Geometry::Point2f& sample_offset) const noexcept override;

    const Geometry::Ray GenerateRayLocalSpace(const Geometry::Point2ui& pixel_coordinates,
                                              const Geometry::Point2f& sample_offset) const noexcept override;

private:
    // Look at matrix
    Geometry::Matrixf look_at;
    // View volume
    Geometry::Point2f bottom_left, top_right;
    // Inverse size of the image
    const float inv_width, inv_height;
};

} // Rabbit namespace

#endif //RABBIT2_PERSPECTIVE_CAMERA_HPP
