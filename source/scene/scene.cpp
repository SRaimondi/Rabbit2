//
// Created by Simon on 2019-04-09.
//

#include "scene.hpp"
#include "light/area_light.hpp"

namespace Rabbit
{

Scene::Scene(Rabbit::BVH&& bvh) noexcept
    : bvh{ std::move(bvh) }
{}

void Scene::SetupAreaLights(unsigned int num_samples) noexcept
{
    for (const Triangle& triangle : bvh.Triangles())
    {
        if (triangle.material->IsEmitting())
        {
            lights.push_back(std::make_unique<const AreaLight>(num_samples, &triangle));
        }
    }
}

} // Rabbit namespace
