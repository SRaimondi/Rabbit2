//
// Created by Simon on 2019-04-09.
//

#ifndef RABBIT2_CONSTANT_TEXTURE_HPP
#define RABBIT2_CONSTANT_TEXTURE_HPP

#include "texture.hpp"

namespace Rabbit
{

template <typename T>
class ConstantTexture : public TextureInterface<T>
{
public:
    constexpr explicit ConstantTexture(const T& value) noexcept
        : value{ value }
    {}

    T Evaluate(const Geometry::Point2f& uv) const noexcept override;

private:
    const T value;
};

template <typename T>
T ConstantTexture<T>::Evaluate(const Rabbit::Geometry::Point2f&) const noexcept
{
    return value;
}

} // Rabbit namespace

#endif //RABBIT2_CONSTANT_TEXTURE_HPP
