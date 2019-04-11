//
// Created by Simon on 2019-04-09.
//

#include "material.hpp"
#include "sampling/montecarlo.hpp"

namespace Rabbit
{

bool MaterialInterface::IsDiffuse() const noexcept
{
    return false;
}

bool MaterialInterface::IsSpecular() const noexcept
{
    return false;
}

bool MaterialInterface::IsEmitting() const noexcept
{
    return false;
}

const Spectrumf MaterialInterface::F(const Geometry::TriangleIntersection&, const Geometry::Vector3f&,
                                     const Geometry::Vector3f&) const noexcept
{
    return Rabbit::Spectrumf{ 0.f };
}

const Spectrumf MaterialInterface::SampleF(const Geometry::TriangleIntersection& intersection,
                                           const Geometry::Vector3f& wo,
                                           const Geometry::Point2f& u, Geometry::Vector3f& sampled_wi,
                                           float& sampled_wi_pdf) const noexcept
{
    // Sample direction using cosine weighted sampling
    const Geometry::Vector3f local_wi{ Sampling::CosineSampleHemisphere(u) };
    // Transform to world space
    sampled_wi = intersection.local_geometry.ToWorld(local_wi);
    // Compute pdf
    sampled_wi_pdf = Sampling::CosineSampleHemispherePdf(Geometry::Framef::CosTheta(local_wi));

    return F(intersection, wo, sampled_wi);
}

const Spectrumf MaterialInterface::Le(const Geometry::TriangleIntersection&, const Geometry::Vector3f&) const noexcept
{
    return Rabbit::Spectrumf{ 0.f };
}

} // Rabbit namespace
