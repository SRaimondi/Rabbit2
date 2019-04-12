//
// Created by simon on 11/04/2019.
//

#include "sampler.hpp"

namespace Rabbit
{
namespace Sampling
{

const std::vector<Geometry::Point2f> Sampler::StratifiedSamples(unsigned int num_samples_dim) noexcept
{
    std::vector<Geometry::Point2f> samples(num_samples_dim * num_samples_dim);
    StratifiedSamples(samples, num_samples_dim);

    return samples;
}

void Sampler::StratifiedSamples(std::vector<Geometry::Point2f>& samples, unsigned int num_samples_dim) noexcept
{
    // Generate stratified pattern
    const float inv_num_samples{ 1.f / num_samples_dim };
    for (unsigned int y = 0; y != num_samples_dim; y++)
    {
        for (unsigned int x = 0; x != num_samples_dim; x++)
        {
            const unsigned int index{ y * num_samples_dim + x };
            samples[index].x = (x + Next1D()) * inv_num_samples;
            samples[index].y = (y + Next1D()) * inv_num_samples;
        }
    }

    // Shuffle samples
    Shuffle(samples.begin(), samples.end());
}

} // Sampling namespace
} // Rabbit namespace