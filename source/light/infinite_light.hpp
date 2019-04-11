//
// Created by simon on 11/04/2019.
//

#ifndef RABBIT2_INFINITE_LIGHT_HPP
#define RABBIT2_INFINITE_LIGHT_HPP

#include "light.hpp"

#include <functional>

namespace Rabbit
{

class InfiniteLight final : public LightInterface
{
public:
    InfiniteLight(unsigned int num_samples, std::function<Spectrumf(const Geometry::Vector3f& direction)>&& rf) noexcept
        : LightInterface{ num_samples }, radiance_function{ std::move(rf) }
    {}

    const Spectrumf SampleLi(const Geometry::TriangleIntersection& reference_intersection, const Geometry::Point2f& u,
                             LightSample& sample, OcclusionTester& occlusion_tester) const noexcept override;

    const Spectrumf L(const Geometry::Ray& ray) const noexcept override;

private:
    const std::function<Spectrumf(const Geometry::Vector3f& direction)> radiance_function;
};

} // Rabbit namespace

#endif //RABBIT2_INFINITE_LIGHT_HPP
