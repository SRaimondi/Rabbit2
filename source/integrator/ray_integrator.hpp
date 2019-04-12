//
// Created by Simon on 2019-04-09.
//

#ifndef RABBIT2_RAY_INTEGRATOR_HPP
#define RABBIT2_RAY_INTEGRATOR_HPP

#include "scene/scene.hpp"
#include "sampling/sampler.hpp"

namespace Rabbit
{

class RayIntegratorInterface
{
public:
    virtual ~RayIntegratorInterface() noexcept = default;

    // Compute incoming radiance for the given ray in the given scene
    virtual const Spectrumf IncomingRadiance(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                                             const Scene& scene, Sampling::Sampler& sampler,
                                             unsigned int depth) const = 0;

protected:
    // Compute direct illumination
    static const Spectrumf ComputeDirectIllumination(const Geometry::TriangleIntersection& intersection,
                                                     const Scene& scene, Sampling::Sampler& sampler) noexcept;

    // Compute specular incoming light
    const Spectrumf ComputeSpecularIllumination(const Geometry::TriangleIntersection& intersection,
                                                const Scene& scene, Sampling::Sampler& sampler,
                                                unsigned int depth) const noexcept;
};

} // Rabbit namespace

#endif //RABBIT2_RAY_INTEGRATOR_HPP
