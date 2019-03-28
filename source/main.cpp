#include <iostream>

#include "opencl/error.hpp"
#include "geometry/mesh.hpp"

int main()
{

    using namespace Geometry;

    try
    {
        // Load bunny mesh
        const Mesh bunny_mesh{ Mesh::LoadOBJ("../models/bunny.obj") };
        // Create list with triangles
        std::vector<Triangle> bunny_triangles{ bunny_mesh.CreateTriangles() };


    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
    }

    return 0;
}