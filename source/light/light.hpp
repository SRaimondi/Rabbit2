//
// Created by Simon on 2019-04-09.
//

#ifndef RABBIT2_LIGHT_HPP
#define RABBIT2_LIGHT_HPP

#include "scene/scene.hpp"

namespace Rabbit
{

// Occlusion tester class
class OcclusionTester
{
public:
    OcclusionTester(const Geometry::Point3f& start, const Geometry::Vector3f& direction) noexcept
        : occlusion_ray{ start, direction }, occlusion_interval{ Geometry::Ray::DefaultInterval() }
    {}

    OcclusionTester(const Geometry::Point3f& start, const Geometry::Point3f& end) noexcept
        : occlusion_ray{ start, end - start },
          occlusion_interval{ Geometry::EPS < float > , 1.f - Geometry::EPS < float > }
    {}

    bool IsOccluded(const Scene& scene) const noexcept
    {
        return scene.IntersectTest(occlusion_ray, occlusion_interval);
    }

private:
    // Ray for test and interval
    const Geometry::Ray occlusion_ray;
    const Geometry::Intervalf occlusion_interval;
};

class LightInterface
{
public:
    // Evaluate incoming light at a given
};

} // Rabbit interface

#endif //RABBIT2_LIGHT_HPP
