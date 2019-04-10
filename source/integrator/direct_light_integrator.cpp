//
// Created by simon on 10/04/2019.
//

#include "direct_light_integrator.hpp"
#include "geometry/occlusion_test.hpp"

namespace Rabbit
{

const Spectrumf DirectLightIntegrator::IncomingRadiance(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                                                        const Scene& scene) const noexcept
{
    Spectrumf L{ 0.f };

    // Intersect ray with the scene
    Geometry::TriangleIntersection intersection;
    if (scene.Intersect(ray, interval, intersection))
    {
        // Loop over all lights in the scene and compute contribution
        for (const auto& light : scene.Lights())
        {
            // Sample incoming light
            Geometry::Vector3f sampled_wi;
            float sampled_wi_pdf{ 0.f };
            OcclusionTester occlusion_tester;
            const Spectrumf Li{ light->SampleLi(intersection, Geometry::Point2f{},
                                                sampled_wi, sampled_wi_pdf, occlusion_tester) };

            // Check if we need to add contribution
            if (!occlusion_tester.IsOccluded(scene) && !Li.IsBlack() && sampled_wi_pdf != 0.f)
            {
                L += intersection.hit_triangle->material->F(intersection, intersection.wo, sampled_wi) *
                     Li * Clamp(Geometry::Dot(intersection.normal, sampled_wi), 0.f, 1.f) / sampled_wi_pdf;
            }
        }
    }

    return L;
}
} // Rabbit namespace
