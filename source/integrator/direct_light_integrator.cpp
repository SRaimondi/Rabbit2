//
// Created by simon on 10/04/2019.
//

#include "direct_light_integrator.hpp"
#include "geometry/occlusion_test.hpp"

namespace Rabbit
{

const Spectrumf DirectLightIntegrator::IncomingRadiance(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                                                        const Scene& scene, Sampling::Sampler& sampler,
                                                        unsigned int depth) const noexcept
{
    Spectrumf L{ 0.f };

    // Intersect ray with the scene
    Geometry::TriangleIntersection intersection;
    if (scene.Intersect(ray, interval, intersection))
    {
        // Add emission from intersection, if any
        if (intersection.hit_triangle->material->IsEmitting())
        {
            L += intersection.hit_triangle->material->Le(intersection, intersection.wo);
        }

        // Add contribution from direct illumination
        L += ComputeDirectIllumination(intersection, scene, sampler);

        // If material is specular, add specular contribution
        if (depth < max_depth)
        {
            L += ComputeSpecularIllumination(intersection, scene, sampler, depth);
        }
    }
    else
    {
        // Add possible contribution from infinite lights
        for (const auto& light : scene.Lights())
        {
            L += light->L(ray);
        }
    }

    return L;
}
} // Rabbit namespace
