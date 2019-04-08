//
// Created by Simon on 2019-04-08.
//

#ifndef RABBIT2_INTERVAL_HPP
#define RABBIT2_INTERVAL_HPP

#include <limits>

namespace Rabbit
{
namespace Geometry
{

template <typename T>
struct Interval
{
    constexpr Interval() noexcept
        : min{ std::numeric_limits<T>::lowest() }, max{ std::numeric_limits<T>::max() }
    {}

    constexpr Interval(T min, T max) noexcept
        : min{ min }, max{ max }
    {}

    constexpr bool IsInsideInclusive(T value) const noexcept
    {
        return value >= min && value <= max;
    }

    constexpr bool IsInsideExclusive(T value) const noexcept
    {
        return value > min && value < max;
    }

    // Minimum and maximum of the interval
    T min, max;
};

} // Geometry namespace
} // Rabbit namespace

#endif //RABBIT2_INTERVAL_HPP
