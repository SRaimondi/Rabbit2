//
// Created by simon on 11/04/2019.
//

#ifndef RABBIT2_OCCLUSION_TEST_HPP
#define RABBIT2_OCCLUSION_TEST_HPP

#include "scene/scene.hpp"

namespace Rabbit
{

// Occlusion tester class
class OcclusionTester
{
public:
    OcclusionTester() noexcept = default;

    void OriginDirection(const Geometry::Point3f& origin, const Geometry::Vector3f& direction) noexcept
    {
        occlusion_ray = Geometry::Ray{ origin, direction };
        occlusion_interval = Geometry::Ray::DefaultInterval();
    }

    void FromTo(const Geometry::Point3f& from, const Geometry::Point3f& to) noexcept
    {
        occlusion_ray = Geometry::Ray{ from, Geometry::Normalize(to - from) };
        occlusion_interval = Geometry::Intervalf{ Geometry::EPS<float>,
                                                  Geometry::Distance(from, to) - Geometry::EPS<float> };
    }

    bool IsOccluded(const Scene& scene) const noexcept
    {
        return scene.IntersectTest(occlusion_ray, occlusion_interval);
    }

private:
    // Ray for test and interval
    Geometry::Ray occlusion_ray;
    Geometry::Intervalf occlusion_interval;
};

}

#endif //RABBIT2_OCCLUSION_TEST_HPP
