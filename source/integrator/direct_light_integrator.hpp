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
    const Spectrumf IncomingRadiance(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                                     const Scene& scene, Sampling::PCG32& rng) const noexcept override;
};

} // Rabbit namespace

#endif //RABBIT2_DIRECT_LIGHT_INTEGRATOR_HPP
