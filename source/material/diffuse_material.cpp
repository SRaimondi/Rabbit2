//
// Created by Simon on 2019-04-09.
//

#include "diffuse_material.hpp"
#include "geometry/common.hpp"

namespace Rabbit
{

DiffuseMaterial::DiffuseMaterial(const std::shared_ptr<const TextureInterface<const Spectrumf>>& r) noexcept
    : reflectance{ r }
{}

bool DiffuseMaterial::IsDiffuse() const noexcept
{
    return true;
}

Spectrumf DiffuseMaterial::F(const Geometry::TriangleIntersection& intersection, const Geometry::Vector3f&,
                             const Geometry::Vector3f&) const noexcept
{
    return reflectance->Evaluate(intersection.uv) * Geometry::INV_PI<float>;
}
} // Rabbit namespace
