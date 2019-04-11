//
// Created by Simon on 2019-04-09.
//

#ifndef RABBIT2_DIFFUSE_MATERIAL_HPP
#define RABBIT2_DIFFUSE_MATERIAL_HPP

#include "material.hpp"
#include "texture/texture.hpp"

#include <memory>

namespace Rabbit
{

class DiffuseMaterial final : public MaterialInterface
{
public:
    explicit DiffuseMaterial(const std::shared_ptr<const TextureInterface<const Spectrumf>>& r) noexcept;

    bool IsDiffuse() const noexcept override;

    const Spectrumf F(const Geometry::TriangleIntersection& intersection,
                      const Geometry::Vector3f& wo, const Geometry::Vector3f& wi) const noexcept override;

private:
    // Reflectance texture
    const std::shared_ptr<const TextureInterface<const Spectrumf>> reflectance;
};

} // Rabbit namespace

#endif //RABBIT2_DIFFUSE_MATERIAL_HPP
