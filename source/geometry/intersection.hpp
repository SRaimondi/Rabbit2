//
// Created by Simon on 2019-03-28.
//

#ifndef RABBIT2_INTERSECTION_HPP
#define RABBIT2_INTERSECTION_HPP

#include "vector.hpp"

namespace Geometry
{

struct alignas(32) Intersection
{
    constexpr Intersection() noexcept = default;

    Vector3 hit_point;
    Vector3 normal;
};

} // Geometry

#endif //RABBIT2_INTERSECTION_HPP
