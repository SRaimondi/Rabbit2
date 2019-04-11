//
// Created by Simon on 2019-04-11.
//

#include "area_light.hpp"
#include "mesh/mesh.hpp"
#include "geometry/occlusion_test.hpp"

namespace Rabbit
{

const Spectrumf AreaLight::SampleLi(const Geometry::TriangleIntersection& reference_intersection,
                                    const Geometry::Point2f& u, LightSample& sample,
                                    OcclusionTester& occlusion_tester) const noexcept
{
    // Sample point on the triangle representing the light
    const Geometry::TriangleIntersection sampled_light{
        triangle->Sample(reference_intersection, u, sample.sampled_wi_pdf) };
    // Compute direction
    sample.sampled_wi = Geometry::Normalize(sampled_light.hit_point - reference_intersection.hit_point);
    occlusion_tester.FromTo(reference_intersection.hit_point, sampled_light.hit_point);

    return triangle->material->Le(sampled_light, -sample.sampled_wi);
}
} // Rabbit namespace
