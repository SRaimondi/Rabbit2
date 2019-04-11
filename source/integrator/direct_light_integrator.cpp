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
        // Get material pointer
        const auto& triangle_material{ intersection.hit_triangle->material };

        // Add possible emission from triangle
        if (triangle_material->IsEmitting())
        {
            L += triangle_material->Le(intersection, intersection.wo);
        }

        LightSample light_sample;
        OcclusionTester occlusion_tester;

        // Loop over all lights in the scene and compute contribution
        for (const auto& light : scene.Lights())
        {
            // Sample incoming light
            const Spectrumf Li{ light->SampleLi(intersection, Geometry::Point2f{}, light_sample, occlusion_tester) };

            // Check if we need to add contribution
            if (!occlusion_tester.IsOccluded(scene) && !Li.IsBlack() && light_sample.sampled_wi_pdf > 0.f)
            {
                L += triangle_material->F(intersection, intersection.wo, light_sample.sampled_wi) *
                     Li * Clamp(Geometry::Dot(intersection.local_geometry.n, light_sample.sampled_wi), 0.f, 1.f) /
                     light_sample.sampled_wi_pdf;
            }
        }
    }

    return L;
}
} // Rabbit namespace
