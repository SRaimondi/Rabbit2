//
// Created by Simon on 2019-03-26.
//

#include "mesh.hpp"
#include "sampling/montecarlo.hpp"

namespace Rabbit
{

Mesh::Mesh(std::vector<Geometry::Point3f>&& v, std::vector<Geometry::Vector3f>&& n,
           std::vector<Geometry::Vector2f>&& uvs, std::vector<TriangleDescription>&& tr)
    : vertices{ std::move(v) }, normals{ std::move(n) }, uvs{ std::move(uvs) }, triangles{ std::move(tr) }
{}

std::vector<Triangle> Mesh::CreateTriangles(const std::shared_ptr<const Geometry::Transform>& transform,
                                            const std::shared_ptr<const MaterialInterface>& material) const noexcept
{
    std::vector<Triangle> mesh_triangles;
    mesh_triangles.reserve(triangles.size());
    for (const TriangleDescription& triangle : triangles)
    {
        mesh_triangles.emplace_back(triangle, *this, transform, material);
    }

    return mesh_triangles;
}

void Mesh::CreateTriangles(const std::shared_ptr<const Geometry::Transform>& transform,
                           const std::shared_ptr<const MaterialInterface>& material,
                           std::vector<Triangle>& triangles_list) const noexcept
{
    for (const TriangleDescription& triangle: triangles)
    {
        triangles_list.emplace_back(triangle, *this, transform, material);
    }
}

Triangle::Triangle(const TriangleDescription& description, const Mesh& m,
                   const std::shared_ptr<const Geometry::Transform>& transform,
                   const std::shared_ptr<const MaterialInterface>& material) noexcept
    : material{ material }, description{ description }, mesh{ m }, transformation{ transform }
{}

const Geometry::TriangleIntersection Triangle::Sample(const Geometry::TriangleIntersection& reference_intersection,
                                                      const Geometry::Point2f& u,
                                                      float& sampled_intersection_pdf) const noexcept
{
    // Get triangle vertices in world space
    const Geometry::Point3f p0{ transformation->ToWorld(mesh.VertexAt(description.v0)) };
    const Geometry::Point3f p1{ transformation->ToWorld(mesh.VertexAt(description.v1)) };
    const Geometry::Point3f p2{ transformation->ToWorld(mesh.VertexAt(description.v2)) };

    // Sample point
    Geometry::TriangleIntersection sampled_intersection;
    sampled_intersection.hit_triangle = this;
    sampled_intersection.barycentric_coordinates = Sampling::UniformSampleTriangle(u);
    sampled_intersection.hit_point = sampled_intersection.barycentric_coordinates.x * p0 +
                                     sampled_intersection.barycentric_coordinates.y * p1 +
                                     sampled_intersection.barycentric_coordinates.z * p2;

    // Compute triangle geometric normal, not normalized
    const Geometry::Vector3f triangle_normal{ Geometry::Cross(p1 - p0, p2 - p0) };
    const float triangle_area{ 0.5f * Geometry::Norm(triangle_normal) };
    sampled_intersection.local_geometry = Geometry::Framef{ Geometry::Normalize(triangle_normal) };

    // Compute UVs for the sampled point
    if (mesh.HasUVs())
    {
        sampled_intersection.uv = sampled_intersection.barycentric_coordinates.x * mesh.UVAt(description.uv0) +
                                  sampled_intersection.barycentric_coordinates.y * mesh.UVAt(description.uv1) +
                                  sampled_intersection.barycentric_coordinates.z * mesh.UVAt(description.uv2);
    }

    // Compute PDF
    const Geometry::Vector3f ref_p_to_sampled_p{ sampled_intersection.hit_point - reference_intersection.hit_point };
    const Geometry::Vector3f wi{ -Geometry::Normalize(ref_p_to_sampled_p) };
    const float n_dot_sampled_wi{ Geometry::Dot(sampled_intersection.local_geometry.n, wi) };
    if (Geometry::SquaredNorm(ref_p_to_sampled_p) == 0.f || n_dot_sampled_wi <= 0.f)
    {
        sampled_intersection_pdf = 0.f;
    }
    else
    {
        sampled_intersection_pdf = Geometry::SquaredNorm(ref_p_to_sampled_p) / (n_dot_sampled_wi * triangle_area);
        if (std::isinf(sampled_intersection_pdf))
        {
            sampled_intersection_pdf = 0.f;
        }
    }

    return sampled_intersection;
}

} // Rabbit namespace
