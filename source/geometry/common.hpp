//
// Created by Simon on 2019-03-29.
//

#ifndef RABBIT2_COMMON_HPP
#define RABBIT2_COMMON_HPP

#include <type_traits>

namespace Rabbit
{
namespace Geometry
{

template <typename T>
constexpr T PI{ 3.1415926535897932385L };

template <typename T>
constexpr T TWO_PI{ T(2) * PI<T> };

template <typename T>
constexpr T PI_OVER_2{ PI<T> / T(2) };

template <typename T>
constexpr T FOUR_PI{ T(4) * PI<T> };

template <typename T>
constexpr T PI_OVER_4{ PI<T> / T(4) };

template <typename T>
constexpr T INV_PI{ T(1) / PI<T> };

template <typename T>
constexpr T INV_2PI{ T(1) / TWO_PI<T> };

template <typename T>
constexpr T INV_4PI{ T(1) / FOUR_PI<T> };

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

} // Geometry namespace
} // Rabbit namespace

#endif //RABBIT2_COMMON_HPP
