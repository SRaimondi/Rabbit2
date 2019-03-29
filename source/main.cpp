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

    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
    }

    return 0;
}