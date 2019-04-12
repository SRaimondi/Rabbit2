//
// Created by Simon on 2019-04-02.
//

#ifndef RABBIT2_SPECTRUM_HPP
#define RABBIT2_SPECTRUM_HPP

#include "utilities/utilities.hpp"

namespace Rabbit
{

template <typename T>
struct Spectrum
{
    constexpr Spectrum() noexcept
        : Spectrum{ 0 }
    {}

    constexpr explicit Spectrum(T v) noexcept
        : Spectrum{ v, v, v }
    {}

    constexpr Spectrum(T r, T g, T b) noexcept
        : r{ r }, g{ g }, b{ b }
    {}

    Spectrum& operator+=(const Spectrum& other) noexcept
    {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }

    Spectrum& operator*=(const Spectrum& other) noexcept
    {
        r *= other.r;
        g *= other.g;
        b *= other.b;
        return *this;
    }

    Spectrum& operator/=(const T t) noexcept
    {
        const T inv_t{ T(1) / t };
        r *= inv_t;
        g *= inv_t;
        b *= inv_t;
        return *this;
    }

    const Spectrum Clamp(T min, T max) const noexcept
    {
        return { ::Rabbit::Clamp(r, min, max), ::Rabbit::Clamp(g, min, max), ::Rabbit::Clamp(b, min, max) };
    }

    constexpr bool IsBlack() const noexcept
    {
        return r == T(0) && g == T(0) && b == T(0);
    }

    // Spectrum components
    T r, g, b;
};

template <typename T>
constexpr const Spectrum<T> operator*(T lhs, const Spectrum<T>& rhs) noexcept
{
    return { lhs * rhs.r, lhs * rhs.g, lhs * rhs.b };
}

template <typename T>
constexpr const Spectrum<T> operator*(const Spectrum<T>& lhs, T rhs) noexcept
{
    return { lhs.r * rhs, lhs.g * rhs, lhs.b * rhs };
}

template <typename T>
constexpr const Spectrum<T> operator*(const Spectrum<T>& lhs, const Spectrum<T>& rhs) noexcept
{
    return { lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b };
}

template <typename T>
constexpr const Spectrum<T> operator/(const Spectrum<T>& lhs, T rhs) noexcept
{
    const T inv_rhs{ 1 / rhs };
    return inv_rhs * lhs;
}

template <typename T>
constexpr T AverageIntensity(const Spectrum<T>& s) noexcept
{
    constexpr T t{ T(1) / T(3) };
    return t * (s.r + s.g + s.b);
}

using Spectrumf = Spectrum<float>;

} // Rabbit namespace

#endif //RABBIT2_SPECTRUM_HPP
