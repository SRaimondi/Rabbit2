//
// Created by Simon on 2019-03-28.
//

#ifndef RABBIT2_INTERSECTION_HPP
#define RABBIT2_INTERSECTION_HPP

#include "vector.hpp"

namespace Geometry
{

struct alignas(32) TriangleIntersection
{
    Vector3 hit_point;
    Vector3 normal;
};

} // Geometry namespace

#endif //RABBIT2_INTERSECTION_HPP
