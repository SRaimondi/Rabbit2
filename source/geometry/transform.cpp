//
// Created by Simon on 2019-04-02.
//

#include "transform.hpp"
#include "common.hpp"

namespace Rabbit
{
namespace Geometry
{

Transform::Transform() noexcept
    : local_to_world{ Matrixf::Identity() }, world_to_local{ Matrixf::Identity() }
{}

Transform::Transform(const Matrixf& l2w)
    : local_to_world{ l2w }, world_to_local{ Inverse(local_to_world) }
{}

Transform::Transform(const Matrixf& l2w, const Matrixf& w2l) noexcept
    : local_to_world{ l2w }, world_to_local{ w2l }
{}

const Transform Translate(float tx, float ty, float tz) noexcept
{
    return { Matrixf{ 1.f, 0.f, 0.f, tx,
                      0.f, 1.f, 0.f, ty,
                      0.f, 0.f, 1.f, tz,
                      0.f, 0.f, 0.f, 1.f },
             Matrixf{ 1.f, 0.f, 0.f, -tx,
                      0.f, 1.f, 0.f, -ty,
                      0.f, 0.f, 1.f, -tz,
                      0.f, 0.f, 0.f, 1.f }};
}

const Transform Translate(const Vector3f& p) noexcept
{
    return Translate(p.x, p.y, p.z);
}

const Transform Scale(float sx, float sy, float sz) noexcept
{
    return { Matrixf{ sx, 0.f, 0.f, 0.f,
                      0.f, sy, 0.f, 0.f,
                      0.f, 0.f, sz, 0.f,
                      0.f, 0.f, 0.f, 1.f },
             Matrixf{ 1.f / sx, 0.f, 0.f, 0.f,
                      0.f, 1.f / sy, 0.f, 0.f,
                      0.f, 0.f, 1.f / sz, 0.f,
                      0.f, 0.f, 0.f, 1.f }};
}

const Transform Scale(float s) noexcept
{
    return Scale(s, s, s);
}

const Transform RotateX(float angle) noexcept
{
    const float sin_t{ std::sin(Radians(angle)) };
    const float cos_t{ std::cos(Radians(angle)) };

    const Matrixf rot{ 1.f, 0.f, 0.f, 0.f,
                       0.f, cos_t, -sin_t, 0.f,
                       0.f, sin_t, cos_t, 0.f,
                       0.f, 0.f, 0.f, 1.f };

    return { rot, Transpose(rot) };
}

const Transform RotateY(float angle) noexcept
{
    const float sin_t{ std::sin(Radians(angle)) };
    const float cos_t{ std::cos(Radians(angle)) };

    const Matrixf rot{ cos_t, 0.f, sin_t, 0.f,
                       0.f, 1.f, 0.f, 0.f,
                       -sin_t, 0.f, cos_t, 0.f,
                       0.f, 0.f, 0.f, 1.f };

    return { rot, Transpose(rot) };
}

const Transform RotateZ(float angle) noexcept
{
    const float sin_t{ std::sin(Radians(angle)) };
    const float cos_t{ std::cos(Radians(angle)) };

    const Matrixf rot{ cos_t, -sin_t, 0.f, 0.f,
                       sin_t, cos_t, 0.f, 0.f,
                       0.f, 0.f, 1.f, 0.f,
                       0.f, 0.f, 0.f, 1.f };

    return { rot, Transpose(rot) };
}

} // Geometry namespace
} // Rabbit namespace
