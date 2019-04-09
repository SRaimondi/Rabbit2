//
// Created by simon on 09/04/2019.
//

#include "debug_integrator.hpp"

namespace Rabbit
{


const Spectrumf DebugIntegrator::IncomingRadiance(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                                                  const Scene& scene) const noexcept
{
    // Intersect ray with scene
    Geometry::TriangleIntersection intersection;
    if (scene.Intersect(ray, interval, intersection))
    {
        // Return color based on debug mode
        switch (debug_mode)
        {
            case DebugMode::HIT:
            {
                return Spectrumf{ 1.f };
            }
            case DebugMode::NORMAL:
            {
                const Geometry::Vector3f mapped_normal{ 0.5f * (intersection.normal + Geometry::Vector3f{ 1.f }) };
                return { mapped_normal.x, mapped_normal.y, mapped_normal.z };
            }
            case DebugMode::UV:
            {
                return { intersection.uv.x, intersection.uv.y, 0.f };
            }
            case DebugMode::WO:
            {
                const Geometry::Vector3f mapped_wo{ 0.5f * (intersection.wo + Geometry::Vector3f{ 1.f }) };
                return { mapped_wo.x, mapped_wo.y, mapped_wo.z };
            }
        }
    }
    else
    {
        return Spectrumf{ 0.f };
    }
}

} // Rabbit namespace
