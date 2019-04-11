//
// Created by simon on 10/04/2019.
//

#include "point_light.hpp"
#include "geometry/occlusion_test.hpp"

namespace Rabbit
{

bool PointLight::IsDeltaLight() const noexcept
{
    return true;
}

const Spectrumf PointLight::SampleLi(const Geometry::TriangleIntersection& reference_intersection,
                                     const Geometry::Point2f&, LightSample& sample,
                                     OcclusionTester& occlusion_tester) const noexcept
{
    sample.sampled_wi = Geometry::Normalize(light_position - reference_intersection.hit_point);
    sample.sampled_wi_pdf = 1.f;
    occlusion_tester.FromTo(reference_intersection.hit_point, light_position);

    return intensity / Geometry::DistanceSquared(reference_intersection.hit_point, light_position);
}
} // Rabbit namespace
