//
// Created by Simon on 2019-04-04.
//

#ifndef RABBIT2_PCG32_HPP
#define RABBIT2_PCG32_HPP

#include <cstdint>

namespace Rabbit
{
namespace Sampling
{

class PCG32
{
public:
    PCG32() noexcept
        : state{ PCG32_DEFAULT_STATE }, stream{ PCG32_DEFAULT_STREAM }
    {}

    explicit PCG32(uint64_t seed, uint64_t init_stream) noexcept
        : PCG32()
    {
        Seed(seed, init_stream);
    }


    // Compute next uniform uint32_t value
    uint32_t NextUInt32() noexcept
    {
        const uint64_t old_state{ state };
        state = old_state * PCG32_MULTIPLIER + (stream | 1u);
        const uint32_t xor_shifted{ static_cast<uint32_t>(((old_state >> 18u) ^ old_state) >> 27u) };
        const uint32_t rot{ static_cast<uint32_t>(old_state >> 59u) };

        return (xor_shifted >> rot) | (xor_shifted << ((-rot) & 31u));
    }

    uint32_t NextUInt32(uint32_t bounds) noexcept
    {
        const uint32_t threshold{ (~bounds + 1u) % bounds };
        while (true)
        {
            uint32_t r{ NextUInt32() };
            if (r >= threshold)
            {
                return r % bounds;
            }
        }
    }

    // Generate a single precision floating point value in the [0, 1) interval
    float NextFloat() noexcept
    {
        union
        {
            uint32_t u;
            float f;
        } p;
        p.u = (NextUInt32() >> 9u) | 0x3f800000u;

        return p.f - 1.f;
    }

private:
    // Seed the generator
    void Seed(uint64_t seed, uint64_t init_stream) noexcept
    {
        state = 0u;
        stream = (init_stream << 1u) | 1u;
        NextUInt32();
        state += seed;
        NextUInt32();
    }

    // Constants for PCG32
    static constexpr uint64_t PCG32_DEFAULT_STATE{ 0x853c49e6748fea9bull };
    static constexpr uint64_t PCG32_DEFAULT_STREAM{ 0xda3e39cb94b95bdbull };
    static constexpr uint64_t PCG32_MULTIPLIER{ 6364136223846793005ull };

    // State and stream
    uint64_t state;
    uint64_t stream;
};

} // Sampling namespace
} // Rabbit namespace

#endif //RABBIT2_PCG32_HPP
