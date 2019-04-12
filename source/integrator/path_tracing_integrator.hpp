//
// Created by Simon on 2019-04-12.
//

#ifndef RABBIT2_PATH_TRACING_INTEGRATOR_HPP
#define RABBIT2_PATH_TRACING_INTEGRATOR_HPP

#include "ray_integrator.hpp"

namespace Rabbit
{

class PathTracingIntegrator final : public RayIntegratorInterface
{
public:
    explicit PathTracingIntegrator(unsigned int max_depth) noexcept
        : max_depth{ max_depth }
    {}

    const Spectrumf IncomingRadiance(const Geometry::Ray& ray, Geometry::Intervalf& interval, const Scene& scene,
                                     Sampling::Sampler& sampler, unsigned int depth) const override;

private:
    // Maximum tracing depth
    const unsigned int max_depth;
};

} // Rabbit namespace

#endif //RABBIT2_PATH_TRACING_INTEGRATOR_HPP
