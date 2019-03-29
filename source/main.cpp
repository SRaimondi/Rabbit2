#include <iostream>

#include "bvh/bvh.hpp"

int main()
{

    using namespace Geometry;

    try
    {
        // Load bunny mesh
        const Mesh bunny_mesh{ Mesh::LoadOBJ("../models/bunny.obj") };

        // Create BVH
        const BVH bvh{ BVHConfig{}, bunny_mesh.CreateTriangles() };

        Ray test_ray{ Vector3{ 0.f, 0.f, 10.f }, Vector3{ 0.f, 0.f, -1.f }};
        TriangleIntersection intersection;

        const bool hit{ bvh.Intersect(test_ray, intersection) };
        std::cout << std::boolalpha;
        std::cout << hit;
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
    }

    return 0;
}