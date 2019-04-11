//
// Created by Simon on 2019-04-09.
//

#ifndef RABBIT2_MATERIAL_HPP
#define RABBIT2_MATERIAL_HPP

#include "film/spectrum.hpp"
#include "geometry/intersection.hpp"

namespace Rabbit
{

// Material sample
struct MaterialSample
{
    constexpr MaterialSample() noexcept
        : sampled_wi_pdf{ 0.f }
    {}

    // Sampled incoming direction
    Geometry::Vector3f sampled_wi;
    // Sampled direction pdf, with respect to solid angle
    float sampled_wi_pdf;
};

class MaterialInterface
{
public:
    virtual ~MaterialInterface() noexcept = default;

    // Checkers for the properties of the Material
    virtual bool IsDiffuse() const noexcept;

    virtual bool IsSpecular() const noexcept;

    virtual bool IsEmitting() const noexcept;

    // Evaluate BRDF value, defaults to 0
    virtual const Spectrumf F(const Geometry::TriangleIntersection& intersection,
                              const Geometry::Vector3f& wo, const Geometry::Vector3f& wi) const noexcept;

    // Sample a new direction for the BRDF, defaults to cosine sampling
    virtual const Spectrumf SampleF(const Geometry::TriangleIntersection& intersection,
                                    const Geometry::Vector3f& wo, const Geometry::Point2f& u,
                                    MaterialSample& sample) const noexcept;

    // Evaluate emission in a given direction, defaults to 0
    virtual const Spectrumf Le(const Geometry::TriangleIntersection& intersection,
                               const Geometry::Vector3f& w) const noexcept;
};

} // Rabbit namespace

#endif //RABBIT2_MATERIAL_HPP
