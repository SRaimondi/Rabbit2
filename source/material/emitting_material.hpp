//
// Created by Simon on 2019-04-09.
//

#ifndef RABBIT2_EMITTING_MATERIAL_HPP
#define RABBIT2_EMITTING_MATERIAL_HPP

#include "material.hpp"
#include "texture/texture.hpp"

#include <memory>

namespace Rabbit
{

class EmittingMaterial : public MaterialInterface
{
public:
    explicit EmittingMaterial(const std::shared_ptr<const TextureInterface<Spectrumf>>& e) noexcept;

    bool IsEmitting() const noexcept override;

    Spectrumf Le(const Geometry::TriangleIntersection& intersection,
                 const Geometry::Vector3f& w) const noexcept override;

private:
    // Emission texture
    std::shared_ptr<const TextureInterface<Spectrumf>> emission;
};

} // Rabbit namespace

#endif //RABBIT2_EMITTING_MATERIAL_HPP