//
// Created by simon on 10/04/2019.
//

#include "direct_light_integrator.hpp"
#include "geometry/occlusion_test.hpp"

namespace Rabbit
{

const Spectrumf DirectLightIntegrator::IncomingRadiance(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                                                        const Scene& scene, Sampling::Sampler& sampler) const noexcept
{
    Spectrumf L{ 0.f };

    // Intersect ray with the scene
    Geometry::TriangleIntersection intersection;
    if (scene.Intersect(ray, interval, intersection))
    {
        // Get material pointer
        const auto& triangle_material{ intersection.hit_triangle->material };

        // Add emission from intersection, if any
        if (triangle_material->IsEmitting())
        {
            L += triangle_material->Le(intersection, intersection.wo);
        }

        LightSample light_sample;
        OcclusionTester occlusion_tester;

        // Loop over all lights in the scene and compute contribution
        for (const auto& light : scene.Lights())
        {
            Spectrumf Ld{ 0.f };
            for (unsigned int sample = 0; sample != light->NumSamples(); sample++)
            {
                // Check if it makes sense to generate a random [0, 1)^2 sample or not
                Geometry::Point2f u{ 0.f };
                if (!light->IsDeltaLight())
                {
                    u = sampler.Next2D();
                }
                const Spectrumf Li{ light->SampleLi(intersection, u, light_sample, occlusion_tester) };

                // Check if we need to add contribution
                if (!Li.IsBlack() && light_sample.sampled_wi_pdf > 0.f && !occlusion_tester.IsOccluded(scene))
                {
                    Ld += triangle_material->F(intersection, intersection.wo, light_sample.sampled_wi) * Li *
                          Clamp(Geometry::Dot(intersection.local_geometry.n, light_sample.sampled_wi), 0.f, 1.f) /
                          light_sample.sampled_wi_pdf;
                }
            }
            L += Ld / static_cast<float>(light->NumSamples());
        }
    }

    return L;
}
} // Rabbit namespace
