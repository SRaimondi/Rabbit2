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
    constexpr explicit Interval(T start = std::numeric_limits<T>::lowest(),
                                T end = std::numeric_limits<T>::max()) noexcept
        : start{ start }, end{ end }
    {}

    constexpr bool IsInsideInclusive(T value) const noexcept
    {
        return value >= start && value <= end;
    }

    constexpr bool IsInsideExclusive(T value) const noexcept
    {
        return value > start && value < end;
    }

    // Minimum and maximum of the interval
    T start, end;
};

using Intervalf = Interval<float>;

} // Geometry namespace
} // Rabbit namespace

#endif //RABBIT2_INTERVAL_HPP
