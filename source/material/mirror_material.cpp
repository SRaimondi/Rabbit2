//
// Created by simon on 11/04/2019.
//

#include "mirror_material.hpp"

namespace Rabbit
{

MirrorMaterial::MirrorMaterial(const std::shared_ptr<const TextureInterface<const Spectrumf>>& r) noexcept
    : reflection{ r }
{}

bool MirrorMaterial::IsSpecular() const noexcept
{
    return true;
}

const Spectrumf MirrorMaterial::SampleF(const Geometry::TriangleIntersection& intersection,
                                        const Geometry::Vector3f& wo, const Geometry::Point2f&,
                                        MaterialSample& sample) const noexcept
{
    sample.sampled_wi = Geometry::Reflect(wo, intersection.local_geometry.n);
    sample.sampled_wi_pdf = 1.f;

    return reflection->Evaluate(intersection.uv);
}

} // Rabbit namespace