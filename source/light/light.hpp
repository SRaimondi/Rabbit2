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
          occlusion_interval{ Geometry::EPS<float>, 1.f - Geometry::EPS<float> }
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
    explicit LightInterface(unsigned int ns = 1u) noexcept
        : num_samples{ ns }
    {}

    // Check if the light is a delta light, defaults to true
    virtual bool IsDeltaLight() const noexcept;

    // Sample incoming light at given intersection
    virtual Spectrumf SampleLi(const Geometry::TriangleIntersection& reference_intersection,
                               const Geometry::Point2f& u, Geometry::Vector3f& sampled_wi, float& sampled_wi_pdf,
                               OcclusionTester& occlusion_tester) const noexcept = 0;

    // Get number of samples for the light
    unsigned int NumSamples() const noexcept
    {
        return num_samples;
    }

protected:
    // Number of samples for this light
    const unsigned int num_samples;
};

} // Rabbit interface

#endif //RABBIT2_LIGHT_HPP
