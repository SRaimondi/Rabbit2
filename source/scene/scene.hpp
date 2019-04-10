//
// Created by Simon on 2019-04-09.
//

#ifndef RABBIT2_SCENE_HPP
#define RABBIT2_SCENE_HPP

#include "bvh/bvh.hpp"
#include "light/light.hpp"

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

    // Add light to the scene
    void AddLight(std::unique_ptr<const LightInterface> light) noexcept
    {
        lights.push_back(std::move(light));
    }

    // Access light
    const std::vector<std::unique_ptr<const LightInterface>>& Lights() const noexcept
    {
        return lights;
    }

private:
    // Accelerator for triangles
    const BVH bvh;
    // List of lights
    std::vector<std::unique_ptr<const LightInterface>> lights;
};

} // Rabbit namespace

#endif //RABBIT2_SCENE_HPP
