//
// Created by simon on 11/04/2019.
//

#ifndef RABBIT2_SAMPLER_HPP
#define RABBIT2_SAMPLER_HPP

#include "pcg32.hpp"
#include "geometry/geometry.hpp"

namespace Rabbit
{
namespace Sampling
{

class Sampler
{
public:
    Sampler() noexcept = default;

    explicit Sampler(uint64_t seed, uint64_t init_stream = 1ull) noexcept
        : rng{ seed, init_stream }
    {}

    // Request next random 1D / 2D float
    float Next1D() noexcept
    {
        return rng.NextFloat();
    }

    const Geometry::Point2f Next2D() noexcept
    {
        return { Next1D(), Next1D() };
    }

private:
    // Random number generator
    PCG32 rng;
};

} // Sampling namespace
} // Rabbit namespace

#endif //RABBIT2_SAMPLER_HPP
