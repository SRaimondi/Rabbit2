//
// Created by Simon on 2019-04-11.
//

#ifndef RABBIT2_AREA_LIGHT_HPP
#define RABBIT2_AREA_LIGHT_HPP

#include "light.hpp"

namespace Rabbit
{

class Triangle;

class AreaLight final : public LightInterface
{
public:
    explicit AreaLight(unsigned int num_samples, const Triangle* const tr) noexcept
        : LightInterface{ num_samples }, triangle{ tr }
    {}

    const Spectrumf SampleLi(const Geometry::TriangleIntersection& reference_intersection, const Geometry::Point2f& u,
                             LightSample& sample, OcclusionTester& occlusion_tester) const noexcept override;

private:
    // Triangle representing light
    const Triangle* const triangle;
};

} // Rabbit namespace

#endif //RABBIT2_AREA_LIGHT_HPP
