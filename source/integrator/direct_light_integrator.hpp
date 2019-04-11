//
// Created by simon on 10/04/2019.
//

#ifndef RABBIT2_DIRECT_LIGHT_INTEGRATOR_HPP
#define RABBIT2_DIRECT_LIGHT_INTEGRATOR_HPP

#include "ray_integrator.hpp"

namespace Rabbit
{

class DirectLightIntegrator final : public RayIntegratorInterface
{
public:
    explicit DirectLightIntegrator(unsigned int max_depth) noexcept
        : max_depth{ max_depth }
    {}

    const Spectrumf IncomingRadiance(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                                     const Scene& scene, Sampling::Sampler& sampler,
                                     unsigned int depth) const noexcept override;

private:
    // Maximum specular depth
    const unsigned int max_depth;
};

} // Rabbit namespace

#endif //RABBIT2_DIRECT_LIGHT_INTEGRATOR_HPP
