#include <iostream>

#include "bvh/bvh.hpp"

int main()
{

    using namespace Geometry;

    try
    {
        // Load bunny mesh
        const Mesh bunny_mesh{ Mesh::LoadOBJ("../models/bunny.obj") };

        // Create list with triangles
        std::vector<Triangle> bunny_triangles{ bunny_mesh.CreateTriangles() };

        // Create BVH
        const BVH bvh{ BVHConfig{}, bunny_triangles };

    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
    }

    return 0;
}