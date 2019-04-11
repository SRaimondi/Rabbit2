//
// Created by simon on 11/04/2019.
//

#include "infinite_light.hpp"
#include "sampling/montecarlo.hpp"
#include "geometry/occlusion_test.hpp"

namespace Rabbit
{

const Spectrumf InfiniteLight::SampleLi(const Geometry::TriangleIntersection& reference_intersection,
                                        const Geometry::Point2f& u, LightSample& sample,
                                        OcclusionTester& occlusion_tester) const noexcept
{
    // Sample a random direction in the unit sphere
    sample.sampled_wi = Sampling::UniformSampleSphere(u);
    sample.sampled_wi_pdf = Sampling::UniformSampleSpherePdf();
    // Setup occlusion tester
    occlusion_tester.OriginDirection(reference_intersection.hit_point, sample.sampled_wi);

    return radiance_function(sample.sampled_wi);
}

const Spectrumf InfiniteLight::L(const Geometry::Ray& ray) const noexcept
{
    return radiance_function(ray.Direction());
}

} // Rabbit
