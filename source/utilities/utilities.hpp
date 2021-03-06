//
// Created by Simon on 2019-04-02.
//

#ifndef RABBIT2_UTILITIES_HPP
#define RABBIT2_UTILITIES_HPP

#include <cstdint>

namespace Rabbit
{

template <typename T>
constexpr const T& Clamp(const T& v, const T& min, const T& max) noexcept
{
    return (v < min ? min : (v > max ? max : v));
}

template <typename T>
constexpr T DivideUp(const T& a, const T& b) noexcept
{
    return (a + b - 1) / b;
}

// Compute base 2 log of value
inline unsigned int Log2Int(uint32_t v) noexcept
{
    return 31 - __builtin_clz(v);
}

} // Rabbit namespace

#endif //RABBIT2_UTILITIES_HPP
