#include "bvh/bvh.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.hpp"

#include <iostream>
#include <chrono>

int main()
{
    using namespace Geometry;

    try
    {
        // Load bunny mesh
        const Mesh bunny_mesh{ Mesh::LoadOBJ("../models/bunny.obj") };

        // Create BVH
        const BVH bvh{ BVHConfig{}, bunny_mesh.CreateTriangles() };

        // Debug rendering process
        constexpr unsigned int WIDTH{ 512 };
        constexpr unsigned int HEIGHT{ 512 };
        std::vector<unsigned char> raster(WIDTH * HEIGHT * 3, 0);

        constexpr unsigned int NUM_TRIALS{ 100 };
        unsigned int num_hits{ 0 };

        const auto start{ std::chrono::high_resolution_clock::now() };
        for (unsigned int trials = 0; trials != NUM_TRIALS; trials++)
        {
            num_hits = 0;
            // Loop over the whole image, generate ray and intersect
            for (unsigned int row = 0; row != HEIGHT; row++)
            {
                for (unsigned int column = 0; column != WIDTH; column++)
                {
                    // Generate ray
                    constexpr Vector3 ray_direction{ 0.f, 0.f, -1.f };
                    Ray ray{ Vector3{ -2.f + 4.f * (column + 0.5f) / WIDTH,
                                      -2.f + 4.f * (row + 0.5f) / HEIGHT,
                                      10.f },
                             ray_direction };

                    // Intersect Ray with BVH
                    TriangleIntersection intersection;
                    if (bvh.Intersect(ray, intersection))
                    {
                        num_hits++;
//                        const unsigned int linear_index{ 3 * (row * WIDTH + column) };
//                        raster[linear_index] = static_cast<unsigned char>(255 * std::abs(intersection.normal.x));
//                        raster[linear_index + 1] = static_cast<unsigned char>(255 * std::abs(intersection.normal.y));
//                        raster[linear_index + 2] = static_cast<unsigned char>(255 * std::abs(intersection.normal.z));
                    }
                }
            }
        }
        const auto end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Found " << num_hits << " hits\n";
        std::cout << "Average time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / NUM_TRIALS << " ms\n";

        // Write image
//        stbi_flip_vertically_on_write(1);
//        stbi_write_png("render.png", WIDTH, HEIGHT, 3, raster.data(), 0);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
    }

    return 0;
}