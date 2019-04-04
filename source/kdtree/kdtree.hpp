//
// Created by Simon on 2019-04-04.
//

#ifndef RABBIT2_KDTREE_HPP
#define RABBIT2_KDTREE_HPP

#include "geometry/mesh.hpp"

namespace Rabbit
{

// Configuration of the KDTree
struct KDTreeConfig
{
    constexpr KDTreeConfig(unsigned int max_triangles_in_leaf,
                           unsigned int intersection_cost,
                           unsigned int traversal_cost,
                           float empty_bonus)
        : max_triangles_in_leaf{ max_triangles_in_leaf }, intersection_cost{ intersection_cost },
          traversal_cost{ traversal_cost },
          empty_bonus{ empty_bonus }
    {}

    // Maximum number of triangles in a leaf node
    const unsigned int max_triangles_in_leaf;
    // Cost of intersection
    const unsigned int intersection_cost;
    // Cost of traversal
    const unsigned int traversal_cost;
    // Bonus for leaving a partition empty
    const float empty_bonus;
};

class KDTree
{
public:
    KDTree(const KDTreeConfig& config, const std::vector<Geometry::Triangle>& tr);

    KDTree(const KDTreeConfig& config, std::vector<Geometry::Triangle>&& tr);

    // Intersect Ray with KDTree
    bool Intersect(Geometry::Ray& ray, Geometry::TriangleIntersection& intersection) const noexcept;

    // Check for intersection
    bool IntersectTest(const Geometry::Ray& ray) const noexcept;

private:
    // KDTree members
    const KDTreeConfig configuration;
    std::vector<Geometry::Triangle> triangles;
};

} // Rabbit namespace

#endif //RABBIT2_KDTREE_HPP
