//
// Created by Simon on 2019-04-09.
//

#ifndef RABBIT2_SCENE_HPP
#define RABBIT2_SCENE_HPP

#include "bvh/bvh.hpp"

namespace Rabbit
{

class Scene
{
public:
    explicit Scene(BVH&& bvh) noexcept;

    // Intersect Ray with Scene
    bool Intersect(const Geometry::Ray& ray, Geometry::Intervalf& interval,
                   Geometry::TriangleIntersection& intersection) const noexcept
    {
        return bvh.Intersect(ray, interval, intersection);
    }

    // Check for intersection
    bool IntersectTest(const Geometry::Ray& ray, const Geometry::Intervalf& interval) const noexcept
    {
        return bvh.IntersectTest(ray, interval);
    }

private:
    // Accelerator for triangles
    const BVH bvh;
};

} // Rabbit namespace

#endif //RABBIT2_SCENE_HPP
