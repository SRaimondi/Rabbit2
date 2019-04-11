//
// Created by simon on 09/04/2019.
//

#ifndef RABBIT2_DEBUG_INTEGRATOR_HPP
#define RABBIT2_DEBUG_INTEGRATOR_HPP

#include "ray_integrator.hpp"

namespace Rabbit
{

// Enum for the type of debug we want to do
enum class DebugMode
{
    HIT,
    NORMAL,
    UV,
    WO
};

class DebugIntegrator : public RayIntegratorInterface
{
public:
    explicit DebugIntegrator(const DebugMode& mode) noexcept
        : debug_mode{ mode }
    {}

    const Spectrumf IncomingRadiance(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                                     const Scene& scene) const override;

private:
    // Debug mode
    const DebugMode debug_mode;
};

} // Rabbit namespace

#endif //RABBIT2_DEBUG_INTEGRATOR_HPP
