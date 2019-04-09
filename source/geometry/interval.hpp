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
class Interval
{
public:
    constexpr explicit Interval(T start = std::numeric_limits<T>::lowest(),
                                T end = std::numeric_limits<T>::max()) noexcept
        : start{ start }, end{ end }
    {}

    constexpr bool IsInsideInclusive(T value) const noexcept
    {
        return value >= Start() && value <= End();
    }

    constexpr bool IsInsideExclusive(T value) const noexcept
    {
        return value > Start() && value < End();
    }

    constexpr T Start() const noexcept
    {
        return start;
    }

    void SetStart(T new_start) noexcept
    {
        start = new_start;
    }

    constexpr T End() const noexcept
    {
        return end;
    }

    void SetEnd(T new_end) noexcept
    {
        end = new_end;
    }

private:
    // Minimum and maximum of the interval
    T start, end;
};

using Intervalf = Interval<float>;

} // Geometry namespace
} // Rabbit namespace

#endif //RABBIT2_INTERVAL_HPP
