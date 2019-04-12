//
// Created by Simon on 2019-04-12.
//

#include "path_tracing_integrator.hpp"

namespace Rabbit
{

const Spectrumf PathTracingIntegrator::IncomingRadiance(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                                                        const Scene& scene, Sampling::Sampler& sampler,
                                                        unsigned int depth) const
{
    // Final computed radiance
    Spectrumf L{ 0.f };
    // Path throughput
    Spectrumf beta{ 1.f };
    // Current ray, initialised with the given one
    Geometry::Ray current_ray{ ray };
    // Same for the interval
    Geometry::Intervalf current_interval{ interval };
    // Flag that tells us if the bounce is specular
    bool specular_bounce{ false };

    // Start tracing
    for (unsigned int bounce = 0; bounce != max_depth; bounce++)
    {
        // Intersect ray with scene
        Geometry::TriangleIntersection intersection;
        const bool intersection_found{ scene.Intersect(current_ray, current_interval, intersection) };

        // Add emitted light if needed
        if (bounce == 0 || specular_bounce)
        {
            // If we hit something, add emitted radiance if any
            if (intersection_found && intersection.hit_triangle->material->IsEmitting())
            {
                L += beta * intersection.hit_triangle->material->Le(intersection, intersection.wo);
            }
            else
            {
                // Add contribution from infinite lights
                for (const auto& light : scene.Lights())
                {
                    L += beta * light->L(current_ray);
                }
            }
        }

        // Terminate path if we escaped
        if (!intersection_found)
        {
            break;
        }

        // Add direct light contribution
        L += beta * ComputeDirectIllumination(intersection, scene, sampler);

        // Sample material to get new direction
        MaterialSample material_sample;
        const Spectrumf f{ intersection.hit_triangle->material->SampleF(intersection, intersection.wo,
                                                                        sampler.Next2D(), material_sample) };
        // Check if it makes sense to continue tracing
        if (f.IsBlack() || material_sample.sampled_wi_pdf == 0.f)
        {
            break;
        }
        // Check if BRDF is specular
        specular_bounce = intersection.hit_triangle->material->IsSpecular();

        // Update path throughput, special care is taken if the BRDF is specular
        const float n_dot_wi{ specular_bounce ?
                              1.f :
                              Clamp(Geometry::Dot(material_sample.sampled_wi, intersection.local_geometry.n), 0.f, 1.f)
        };
        beta *= f * n_dot_wi / material_sample.sampled_wi_pdf;

        // Set ray to new one
        current_ray = intersection.SpawnRay(material_sample.sampled_wi);
        current_interval = Geometry::Ray::DefaultInterval();

        // Possibly terminate using Russian roulette
        if (bounce > 3)
        {
            // Compute probability to stop
            const float q{ std::max(0.05f, 1.f - AverageIntensity(beta)) };
            if (sampler.Next1D() < q)
            {
                break;
            }
            beta /= 1.f - q;
        }
    }

    return L;
}

} // Rabbit namespace
