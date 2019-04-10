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

    void OriginDirection(const Geometry::Point3f& start, const Geometry::Vector3f& direction) noexcept
    {
        occlusion_ray = Geometry::Ray{ start, direction };
        occlusion_interval = Geometry::Ray::DefaultInterval();
    }

    void FromTo(const Geometry::Point3f& start, const Geometry::Point3f& end) noexcept
    {
        occlusion_ray = Geometry::Ray{ start, Geometry::Normalize(end - start) };
        occlusion_interval = Geometry::Intervalf{ Geometry::EPS<float>,
                                                  Geometry::Distance(start, end) - Geometry::EPS<float> };
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
