//
// Created by Simon on 2019-04-09.
//

#ifndef RABBIT2_MATERIAL_HPP
#define RABBIT2_MATERIAL_HPP

#include "film/spectrum.hpp"
#include "geometry/intersection.hpp"

namespace Rabbit
{

class MaterialInterface
{
public:
    // Checkers for the properties of the Material
    virtual bool IsDiffuse() const noexcept;

    virtual bool IsSpecular() const noexcept;

    virtual bool IsEmitting() const noexcept;

    // Evaluate BRDF value, defaults to 0
    virtual Spectrumf F(const Geometry::TriangleIntersection& intersection,
                        const Geometry::Vector3f& wo, const Geometry::Vector3f& wi) const noexcept;

    // Sample a new direction for the BRDF, defaults to cosine sampling
    virtual Spectrumf SampleF(const Geometry::TriangleIntersection& intersection,
                              const Geometry::Vector3f& wo, const Geometry::Point2f& u,
                              Geometry::Vector3f& sampled_wi, float& sampled_wi_pdf) const noexcept;

    // Evaluate emission in a given direction, defaults to 0
    virtual Spectrumf Le(const Geometry::TriangleIntersection& intersection,
                         const Geometry::Vector3f& w) const noexcept;
};

} // Rabbit namespace

#endif //RABBIT2_MATERIAL_HPP
