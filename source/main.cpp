#include <iostream>

#include "opencl/error.hpp"
#include "geometry/mesh.hpp"

int main()
{
    try
    {
        const Geometry::Mesh bunny_mesh{ Geometry::Mesh::LoadOBJ("../models/bunny.obj") };

        int x = 0;
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
    }

    return 0;
}