//
// Created by Simon on 2019-04-09.
//

#ifndef RABBIT2_RAY_INTEGRATOR_HPP
#define RABBIT2_RAY_INTEGRATOR_HPP

#include "scene/scene.hpp"
#include "sampling/pcg32.hpp"

namespace Rabbit
{

class RayIntegratorInterface
{
public:
    virtual ~RayIntegratorInterface() noexcept = default;

    // Compute incoming radiance for the given ray in the given scene
    virtual const Spectrumf IncomingRadiance(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                                             const Scene& scene, Sampling::PCG32& rng) const = 0;
};

} // Rabbit namespace

#endif //RABBIT2_RAY_INTEGRATOR_HPP
