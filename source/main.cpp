#include <iostream>

#include "opencl/error.hpp"
#include "geometry/mesh.hpp"

int main()
{

    using namespace Geometry;

    try
    {
        Geometry::BBox b;
        Geometry::BBox b2{ Geometry::Vector3{ -1.f }, Geometry::Vector3{ 1.f }};

        Geometry::BBox b3{{ Vector3{ 1.f, 2.f, 1.f }, Vector3{ -1.f, 3.f, -5.f }, Vector3{ 0.f, 0.1f, 4.f }}};

        const Geometry::Mesh bunny_mesh{ Geometry::Mesh::LoadOBJ("../models/bunny.obj") };
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
    }

    return 0;
}