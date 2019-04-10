//
// Created by Simon on 2019-04-09.
//

#include "scene.hpp"
#include "light/light.hpp"

namespace Rabbit
{

Scene::Scene(Rabbit::BVH&& bvh) noexcept
    : bvh{ std::move(bvh) }
{}

} // Rabbit namespace
