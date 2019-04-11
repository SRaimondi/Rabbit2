//
// Created by Simon on 2019-04-11.
//

#include "ray_integrator.hpp"
#include "geometry/occlusion_test.hpp"

namespace Rabbit
{

const Spectrumf ComputeDirectIllumination(const Geometry::TriangleIntersection& intersection, const Scene& scene,
                                          Sampling::Sampler& sampler) noexcept
{
    Spectrumf L{ 0.f };

    // Loop over all lights in the scene and compute contribution
    LightSample light_sample;
    OcclusionTester occlusion_tester;
    for (const auto& light : scene.Lights())
    {
        // Estimate direct illumination for the light
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
                Ld += intersection.hit_triangle->material->F(intersection, intersection.wo, light_sample.sampled_wi) *
                      Li * Clamp(Geometry::Dot(intersection.local_geometry.n, light_sample.sampled_wi), 0.f, 1.f) /
                      light_sample.sampled_wi_pdf;
            }
        }
        L += Ld / static_cast<float>(light->NumSamples());
    }

    return L;
}

} // Rabbit namespace

