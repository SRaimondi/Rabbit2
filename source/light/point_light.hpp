//
// Created by simon on 10/04/2019.
//

#ifndef RABBIT2_POINT_LIGHT_HPP
#define RABBIT2_POINT_LIGHT_HPP

#include "light.hpp"

namespace Rabbit
{

class PointLight : public LightInterface
{
public:
    PointLight(const Geometry::Point3f& position, const Spectrumf& i) noexcept
        : light_position{ position }, intensity{ i }
    {}

    bool IsDeltaLight() const noexcept override;

    Spectrumf SampleLi(const Geometry::TriangleIntersection& reference_intersection, const Geometry::Point2f& u,
                       Geometry::Vector3f& sampled_wi, float& sampled_wi_pdf,
                       OcclusionTester& occlusion_tester) const noexcept override;

private:
    // Light position and intensity
    const Geometry::Point3f light_position;
    const Spectrumf intensity;
};

} // Rabbit namespace

#endif //RABBIT2_POINT_LIGHT_HPP
