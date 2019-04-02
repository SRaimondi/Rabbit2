//
// Created by Simon on 2019-04-02.
//

#ifndef RABBIT2_SPECTRUM_HPP
#define RABBIT2_SPECTRUM_HPP

#include "utilities.hpp"

namespace Rabbit
{

template <typename T>
struct Spectrum
{
    constexpr Spectrum() noexcept
        : r{ 0 }, g{ 0 }, b{ 0 }
    {}

    constexpr explicit Spectrum(T v) noexcept
        : r{ v }, g{ v }, b{ 0 }
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

    const Spectrum Clamp(T min, T max) const noexcept
    {
        return { ::Rabbit::Clamp(r, min, max), ::Rabbit::Clamp(g, min, max), ::Rabbit::Clamp(b, min, max) };
    }

    // Spectrum components
    T r, g, b;
};

using Spectrumf = Spectrum<float>;

} // Rabbit namespace

#endif //RABBIT2_SPECTRUM_HPP
