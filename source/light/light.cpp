//
// Created by simon on 09/04/2019.
//

#include "light.hpp"

namespace Rabbit
{

bool Rabbit::LightInterface::IsDeltaLight() const noexcept
{
    return false;
}

const Spectrumf LightInterface::L(const Geometry::Ray&) const noexcept
{
    return Rabbit::Spectrumf{ 0.f };
}

} // Rabbit namespace
