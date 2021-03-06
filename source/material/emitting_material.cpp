//
// Created by Simon on 2019-04-09.
//

#include "emitting_material.hpp"

namespace Rabbit
{
EmittingMaterial::EmittingMaterial(const std::shared_ptr<const TextureInterface<const Spectrumf>>& e) noexcept
    : emission{ e }
{}

bool EmittingMaterial::IsEmitting() const noexcept
{
    return true;
}

const Spectrumf EmittingMaterial::Le(const Geometry::TriangleIntersection& intersection,
                                     const Geometry::Vector3f& w) const noexcept
{
    return Geometry::Dot(intersection.local_geometry.n, w) > 0.f ?
           emission->Evaluate(intersection.uv) :
           Spectrumf{ 0.f };
}

} // Rabbit namespace
