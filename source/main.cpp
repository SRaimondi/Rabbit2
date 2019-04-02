#include "bvh/bvh.hpp"
#include "camera/camera.hpp"
#include "geometry/common.hpp"

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
        const auto mesh_read_start{ std::chrono::high_resolution_clock::now() };
        const Mesh dragon1_mesh{ Mesh::LoadPLY("../models/dragon1.ply") };
        const Mesh dragon2_mesh{ Mesh::LoadPLY("../models/dragon2.ply") };
        // const Mesh statue_mesh{ Mesh::LoadPLY("../models/statue.ply") };
        const auto mesh_read_end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Read mesh in "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(mesh_read_end - mesh_read_start).count()
                  << " ms\n";

        std::vector<Triangle> dragon1_triangles{ dragon1_mesh.CreateTriangles() };
        std::vector<Triangle> dragon2_triangles{ dragon2_mesh.CreateTriangles() };
        //std::vector<Triangle> statue_triangles{ statue_mesh.CreateTriangles() };

        std::vector<Triangle> scene_triangles;
        std::move(dragon1_triangles.begin(), dragon1_triangles.end(), std::back_inserter(scene_triangles));
        std::move(dragon2_triangles.begin(), dragon2_triangles.end(), std::back_inserter(scene_triangles));
        // std::move(statue_triangles.begin(), statue_triangles.end(), std::back_inserter(scene_triangles));

        // Create BVH
        const auto bvh_start{ std::chrono::high_resolution_clock::now() };
        const BVH bvh{ BVHConfig{ 4, 1.f, 0.2f, 64 }, std::move(scene_triangles) };
        const auto bvh_end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Built BVH in "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(bvh_end - bvh_start).count() << " ms\n";

        constexpr unsigned int WIDTH{ 1000 };
        constexpr unsigned int HEIGHT{ 1000 };

        // Create camera
        const Camera camera{ Point3f{ -3.f, 2.f, 6.f }, Point3f{}, Vector3f{ 0.f, 1.f, 0.f },
                             60.f, WIDTH, HEIGHT };

        // Performance rendering process
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
                    Ray ray{ camera.GenerateRay(Point2ui{ column, row }, Point2f{ 0.5f }) };

                    // Intersect Ray with BVH
                    TriangleIntersection intersection;
                    if (bvh.Intersect(ray, intersection))
                    {
                        num_hits++;
                        const unsigned int linear_index{ 3 * (row * WIDTH + column) };
                        const float n_dot_wo{ Geometry::Clamp(Dot(intersection.normal, -ray.direction), 0.f, 1.f) };
                        raster[linear_index] = raster[linear_index + 1] =
                        raster[linear_index + 2] = static_cast<unsigned char>(255 * n_dot_wo);
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