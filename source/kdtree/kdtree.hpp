//
// Created by Simon on 2019-04-04.
//

#ifndef RABBIT2_KDTREE_HPP
#define RABBIT2_KDTREE_HPP

#include "geometry/mesh.hpp"

namespace Rabbit
{

// KDTree node representation structure
struct KDTreeNode
{
    // Create leaf node
    KDTreeNode(const std::vector<unsigned int>& leaf_triangles, std::vector<unsigned int>& triangle_indices) noexcept
        : flags{ 3 }
    {
        // Set the number of triangles
        num_triangles |= (static_cast<uint32_t>(leaf_triangles.size()) << 2u);
        // Store triangles indices for leaf
        if (leaf_triangles.empty())
        {
            single_triangle = 0;
        }
        else if (leaf_triangles.size() == 1)
        {
            single_triangle = leaf_triangles[0];
        }
        else
        {
            triangle_indices_offset = static_cast<uint32_t>(triangle_indices.size());
            for (unsigned int i : leaf_triangles)
            {
                triangle_indices.emplace_back(i);
            }
        }
    }

    // Create leaf node
    KDTreeNode(float split_pos, unsigned int axis, unsigned int above_child) noexcept
        : split_position{ split_pos }, flags{ axis }
    {
        above_child_index |= (static_cast<uint32_t>(above_child) << 2u);
    }

    // Access node fields and hide the internal representation
    float SplitPosition() const noexcept
    {
        return split_position;
    }

    uint32_t NumTriangles() const noexcept
    {
        return num_triangles >> 2u;
    }

    uint32_t SplitAxis() const noexcept
    {
        return flags & 3u;
    }

    bool IsLeaf() const noexcept
    {
        return SplitAxis() == 3u;
    }

    uint32_t AboveChild() const noexcept
    {
        return above_child_index >> 2u;
    }

    union
    {
        float split_position;
        uint32_t single_triangle;           // If the node overlaps only a single triangle, this is the index
        uint32_t triangle_indices_offset;   // Offset in the indices of triangles where the triangles start
    };
    union
    {
        uint32_t flags;                     // We use the first 2 bits to either store the split axis (0, 1, 2) or, if the node is a leaf, 3
        uint32_t num_triangles;             // Number of primitives is in the upper 30 bits
        uint32_t above_child_index;
    };
};

// Configuration of the KDTree
struct KDTreeConfig
{
    constexpr explicit KDTreeConfig(unsigned int max_triangles_in_leaf = 1u,
                                    unsigned int intersection_cost = 80u,
                                    unsigned int traversal_cost = 1u,
                                    float empty_bonus = 0.5f)
        : max_triangles_in_leaf{ max_triangles_in_leaf },
          intersection_cost{ intersection_cost },
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
    // Build KDTree
    void Build();

    // KDTree building configuration
    const KDTreeConfig configuration;
    // Triangles stored in the tree
    std::vector<Geometry::Triangle> triangles;
    // Indices of the triangles in a leaf node
    std::vector<unsigned int> triangle_indices;
    // Flat nodes
    KDTreeNode* nodes;
    // Number of allocated nodes and index of the next node to write to
    unsigned int num_allocated_nodes, next_free_node;
    // Bounds of the tree
    Geometry::BBox tree_bounds;
};

} // Rabbit namespace

#endif //RABBIT2_KDTREE_HPP
