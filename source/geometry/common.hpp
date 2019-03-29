//
// Created by Simon on 2019-03-29.
//

#ifndef RABBIT2_COMMON_HPP
#define RABBIT2_COMMON_HPP

#include <type_traits>

namespace Geometry
{

template <typename T>
constexpr T PI{ 3.1415926535897932385L };

// Convert radians to degrees
template <typename T, typename std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
constexpr T Radians(const T& deg) noexcept
{
    return deg * (PI<T> / T(180));
}

// Convert degrees to radians
template <typename T, typename std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
constexpr T Degrees(const T& rad) noexcept
{
    return rad * (T(180) / PI<T>);
}

template <typename T>
constexpr const T& Clamp(const T& v, const T& min, const T& max) noexcept
{
    return (v < min ? min : (v > max ? max : v));
}

} // Geometry namespace

#endif //RABBIT2_COMMON_HPP
