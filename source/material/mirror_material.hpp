//
// Created by simon on 11/04/2019.
//

#ifndef RABBIT2_MIRROR_MATERIAL_HPP
#define RABBIT2_MIRROR_MATERIAL_HPP

#include "material.hpp"
#include "texture/texture.hpp"

#include <memory>

namespace Rabbit
{

class MirrorMaterial final : public MaterialInterface
{
public:
    explicit MirrorMaterial(const std::shared_ptr<const TextureInterface<const Spectrumf>>& r) noexcept;

    bool IsSpecular() const noexcept override;

    const Spectrumf SampleF(const Geometry::TriangleIntersection& intersection, const Geometry::Vector3f& wo,
                            const Geometry::Point2f& u, MaterialSample& sample) const noexcept override;

private:
    // Reflection value
    const std::shared_ptr<const TextureInterface<const Spectrumf>> reflection;
};

} // Rabbit namespace

#endif //RABBIT2_MIRROR_MATERIAL_HPP
