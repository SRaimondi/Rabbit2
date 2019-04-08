//
// Created by Simon on 2019-04-04.
//

#include "kdtree.hpp"
#include "utilities/utilities.hpp"

namespace Rabbit
{

KDTree::KDTree(const KDTreeConfig& config, const std::vector<Geometry::Triangle>& tr)
    : configuration{ config }, triangles{ tr },
      nodes{ nullptr }, num_allocated_nodes{ 0 }, next_free_node{ 0 }
{
    Build();
}

KDTree::KDTree(const KDTreeConfig& config, std::vector<Geometry::Triangle>&& tr)
    : configuration{ config }, triangles{ std::move(tr) },
      nodes{ nullptr }, num_allocated_nodes{ 0 }, next_free_node{ 0 }
{
    Build();
}

bool KDTree::Intersect(Geometry::Ray& ray, Geometry::TriangleIntersection& intersection) const noexcept
{
    return false;
}

bool KDTree::IntersectTest(const Geometry::Ray& ray) const noexcept
{
    return false;
}

void KDTree::Build()
{
    // Determine maximum depth of the tree
    const unsigned int max_depth{
        static_cast<unsigned int>(std::round(8 + 1.3f * Log2Int(static_cast<uint32_t>(triangles.size())))) };
}

} // Rabbit namespace
