//
// Created by Simon on 2019-04-04.
//

#include "kdtree.hpp"

namespace Rabbit
{

KDTree::KDTree(const KDTreeConfig& config, const std::vector<Geometry::Triangle>& tr)
    : configuration{ config }, triangles{ tr }
{

}

KDTree::KDTree(const KDTreeConfig& config, std::vector<Geometry::Triangle>&& tr)
    : configuration{ config }, triangles{ std::move(tr) }
{

}

bool KDTree::Intersect(Geometry::Ray& ray, Geometry::TriangleIntersection& intersection) const noexcept
{
    return false;
}

bool KDTree::IntersectTest(const Geometry::Ray& ray) const noexcept
{
    return false;
}

} // Rabbit namespace
