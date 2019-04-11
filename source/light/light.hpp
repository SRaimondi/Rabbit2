//
// Created by Simon on 2019-04-09.
//

#ifndef RABBIT2_LIGHT_HPP
#define RABBIT2_LIGHT_HPP

#include "geometry/intersection.hpp"
#include "film/spectrum.hpp"

namespace Rabbit
{

// Forward declare OcclusionTester to avoid circular inclusion
class OcclusionTester;

// Light sample
struct LightSample
{
    constexpr LightSample() noexcept
        : sampled_wi_pdf{ 0.f }
    {}

    // Sampled light direction
    Geometry::Vector3f sampled_wi;
    // Sampled direction pdf, with respect to solid angle
    float sampled_wi_pdf;
};

class LightInterface
{
public:
    explicit LightInterface(unsigned int ns = 1u) noexcept
        : num_samples{ ns }
    {}

    virtual ~LightInterface() noexcept = default;

    // Check if the light is a delta light, defaults to true
    virtual bool IsDeltaLight() const noexcept;

    // Sample incoming light at given intersection
    virtual const Spectrumf SampleLi(const Geometry::TriangleIntersection& reference_intersection,
                                     const Geometry::Point2f& u, LightSample& sample,
                                     OcclusionTester& occlusion_tester) const noexcept = 0;

    // Get number of samples for the light
    unsigned int NumSamples() const noexcept
    {
        return num_samples;
    }

protected:
    // Number of samples for this light
    const unsigned int num_samples;
};

} // Rabbit interface

#endif //RABBIT2_LIGHT_HPP
