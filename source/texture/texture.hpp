//
// Created by Simon on 2019-04-09.
//

#ifndef RABBIT2_TEXTURE_HPP
#define RABBIT2_TEXTURE_HPP

#include "geometry/geometry.hpp"

namespace Rabbit
{

template <typename T>
class TextureInterface
{
public:
    virtual ~TextureInterface() = default;

    // Evaluate texture at given uv coordinates
    virtual T Evaluate(const Geometry::Vector2f& uv) const noexcept = 0;
};

} // Rabbit namespace

#endif //RABBIT2_TEXTURE_HPP
