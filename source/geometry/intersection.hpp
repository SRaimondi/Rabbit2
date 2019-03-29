//
// Created by Simon on 2019-03-28.
//

#ifndef RABBIT2_INTERSECTION_HPP
#define RABBIT2_INTERSECTION_HPP

#include "vector.hpp"

namespace Geometry
{

struct TriangleIntersection
{
    // Hit point
    Vector3 hit_point;
    // Barycentric coordinate for vertex v0
    float u;
    // Normal at hit point
    Vector3 normal;
    // Barycentric coordinate for vertex v1 and v2
    float v, w;
    // Index of the triangle that generated the intersection
    unsigned int triangle_index;
};

} // Geometry namespace

#endif //RABBIT2_INTERSECTION_HPP
