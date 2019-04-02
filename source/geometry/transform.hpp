//
// Created by Simon on 2019-04-02.
//

#ifndef RABBIT2_TRANSFORM_HPP
#define RABBIT2_TRANSFORM_HPP

#include "matrix.hpp"
#include "ray.hpp"

namespace Rabbit
{
namespace Geometry
{

class Transform
{
public:
    Transform() noexcept;

    explicit Transform(const Matrixf& l2w);

    Transform(const Matrixf& l2w, const Matrixf& w2l) noexcept;

    const Point3f ToWorld(const Point3f& p) const noexcept
    {
        return local_to_world(p);
    }

    const Vector3f ToWorld(const Vector3f& v) const noexcept
    {
        return local_to_world(v);
    }

    const Vector3f NormalToWorld(const Vector3f& n) const noexcept
    {
        return world_to_local.TransformNormal(n);
    }

    const Ray ToLocal(const Ray& ray) const noexcept
    {
        return { world_to_local(ray.origin), world_to_local(ray.direction), ray.extent_start, ray.extent_end };
    }

    // Compose transformation
    const Transform operator*(const Transform& other) const noexcept
    {
        // Left multiply the local to world and right multiply world to local since its the inverse
        return { local_to_world * other.local_to_world, other.world_to_local * world_to_local };
    }

private:
    // Local to world and world to local matrices
    const Matrixf local_to_world;
    const Matrixf world_to_local;
};

// Generate translation
const Transform Translate(float tx, float ty, float tz) noexcept;

const Transform Translate(const Vector3f& p) noexcept;

// Generate scaling
const Transform Scale(float sx, float sy, float sz) noexcept;

const Transform Scale(float s) noexcept;

// Rotation
const Transform RotateX(float angle) noexcept;

const Transform RotateY(float angle) noexcept;

const Transform RotateZ(float angle) noexcept;

} // Geometry namespace
} // Rabbit namespace

#endif //RABBIT2_TRANSFORM_HPP
