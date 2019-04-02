#include "bvh/bvh.hpp"
#include "camera/camera.hpp"
#include "geometry/common.hpp"
#include "geometry/matrix.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.hpp"

#include <iostream>
#include <chrono>

int main()
{
    using namespace Geometry;

    Matrixf m{ 1.0, 2.f, 3.f, 4.f,
               7.f, 8.f, 9.f, 11.f,
               14.f, -1.f, 4.f, 90.f,
               13.f, -9.f, 5.f, 6.f };

    const Matrixf inv_m{ Inverse(m) };

    try
    {
        // Load bunny mesh
        const auto mesh_read_start{ std::chrono::high_resolution_clock::now() };
        const Mesh dragon{ Mesh::LoadPLY("../models/dragon.ply") };
        const auto mesh_read_end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Read mesh in "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(mesh_read_end - mesh_read_start).count()
                  << " ms\n";

        const Transform scale{ Scale(0.5f) };
        const auto tr1{ std::make_shared<const Transform>(Translate(3.f, 0.f, 0.f) * RotateY(30.f)) };
        const auto tr2{ std::make_shared<const Transform>(RotateZ(90.f) * RotateY(90.f)) };
        const auto tr3{ std::make_shared<const Transform>(Translate(-3.f, 0.f, 0.f) * RotateY(120.f)) };
        std::vector<Triangle> dragon1_triangles{ dragon.CreateTriangles(tr1) };
        std::vector<Triangle> dragon2_triangles{ dragon.CreateTriangles(tr2) };
        std::vector<Triangle> dragon3_triangles{ dragon.CreateTriangles(tr3) };

        std::vector<Triangle> scene_triangles;
        std::move(dragon1_triangles.begin(), dragon1_triangles.end(), std::back_inserter(scene_triangles));
        std::move(dragon2_triangles.begin(), dragon2_triangles.end(), std::back_inserter(scene_triangles));
        std::move(dragon3_triangles.begin(), dragon3_triangles.end(), std::back_inserter(scene_triangles));

        // Create BVH
        const auto bvh_start{ std::chrono::high_resolution_clock::now() };
        const BVH bvh{ BVHConfig{ 4, 1.f, 0.2f, 64 }, std::move(scene_triangles) };
        const auto bvh_end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Built BVH in "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(bvh_end - bvh_start).count() << " ms\n";

        constexpr unsigned int WIDTH{ 1000 };
        constexpr unsigned int HEIGHT{ 1000 };

        // Create camera
        const Camera camera{ Point3f{ 0.f, 3.f, 10.f }, Point3f{}, Vector3f{ 0.f, 1.f, 0.f },
                             60.f, WIDTH, HEIGHT };

        // Performance rendering process
        std::vector<unsigned char> raster(WIDTH * HEIGHT * 3, 0);

        constexpr unsigned int NUM_TRIALS{ 1 };
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
                    Ray ray{ camera.GenerateRayWorldSpace(Point2ui{ column, row }, Point2f{ 0.5f }) };

                    // Intersect Ray with BVH
                    TriangleIntersection intersection;
                    if (bvh.Intersect(ray, intersection))
                    {
                        num_hits++;
                        const unsigned int linear_index{ 3 * (row * WIDTH + column) };
                        raster[linear_index] = static_cast<unsigned char>(255 * std::abs(intersection.normal.x));
                        raster[linear_index + 1] = static_cast<unsigned char>(255 * std::abs(intersection.normal.y));
                        raster[linear_index + 2] = static_cast<unsigned char>(255 * std::abs(intersection.normal.z));
                    }
                }
            }
        }
        const auto end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Found " << num_hits << " hits\n";
        std::cout << "Average rendering time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / NUM_TRIALS << " ms\n";

        // Write image
        stbi_flip_vertically_on_write(1);
        stbi_write_png("render.png", WIDTH, HEIGHT, 3, raster.data(), 0);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
    }

    return 0;
}