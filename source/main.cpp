#include "bvh/bvh.hpp"
#include "mesh/mesh_loader.hpp"
#include "camera/camera.hpp"
#include "film/film.hpp"
#include "geometry/common.hpp"
#include "sampling/pcg32.hpp"

#include <iostream>
#include <chrono>

int main()
{
    using namespace Rabbit;
    using namespace Rabbit::Geometry;

    try
    {
        // Load bunny mesh
        const auto mesh_read_start{ std::chrono::high_resolution_clock::now() };
        const Mesh dragon{ LoadMesh("../models/dragon.ply") };
        const Mesh plane{ LoadMesh("../models/plane.ply") };
        const auto mesh_read_end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Read meshes in "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(mesh_read_end - mesh_read_start).count()
                  << " ms\n";

        const auto tr_plane{
            std::make_shared<const Transform>(Translate(0.f, -1.16f, 0.f) * Scale(200.f, 1.f, 200.f)) };
        const auto tr1{ std::make_shared<const Transform>(Translate(3.f, 0.f, 0.f) * RotateY(90.f)) };
        const auto tr2{ std::make_shared<const Transform>(RotateY(90.f)) };
        const auto tr3{ std::make_shared<const Transform>(Translate(-3.f, 0.f, 0.f) * RotateY(90.f)) };

        std::vector<Triangle> plane_triangles{ plane.CreateTriangles(tr_plane) };
        std::vector<Triangle> dragon1_triangles{ dragon.CreateTriangles(tr1) };
        std::vector<Triangle> dragon2_triangles{ dragon.CreateTriangles(tr2) };
        std::vector<Triangle> dragon3_triangles{ dragon.CreateTriangles(tr3) };

        std::vector<Triangle> scene_triangles;
        std::move(plane_triangles.begin(), plane_triangles.end(), std::back_inserter(scene_triangles));
        std::move(dragon1_triangles.begin(), dragon1_triangles.end(), std::back_inserter(scene_triangles));
        std::move(dragon2_triangles.begin(), dragon2_triangles.end(), std::back_inserter(scene_triangles));
        std::move(dragon3_triangles.begin(), dragon3_triangles.end(), std::back_inserter(scene_triangles));

        // Create BVH
        const auto bvh_start{ std::chrono::high_resolution_clock::now() };
        const BVH bvh{ BVHConfig{ 4, 1.f, 0.2f, 128 }, scene_triangles };
        const auto bvh_end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Built BVH in "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(bvh_end - bvh_start).count() << " ms\n";

        constexpr unsigned int WIDTH{ 800 };
        constexpr unsigned int HEIGHT{ 800 };
        constexpr unsigned int NUM_SAMPLES{ 32 };
        constexpr float INV_SAMPLES{ 1.f / NUM_SAMPLES };

        Film film{ WIDTH, HEIGHT };

        // Create camera
        const Camera camera{ Point3f{ 5.f, 3.f, 5.f }, Point3f{}, Vector3f{ 0.f, 1.f, 0.f },
                             60.f, WIDTH, HEIGHT };

        // Performance rendering process
        constexpr unsigned int NUM_TRIALS{ 1 };

        // Random number generator
        Sampling::PCG32 rng;

        const auto start{ std::chrono::high_resolution_clock::now() };
        for (unsigned int trials = 0; trials != NUM_TRIALS; trials++)
        {
            // Loop over the whole image, generate ray and intersect
            for (unsigned int row = 0; row != HEIGHT; row++)
            {
                for (unsigned int column = 0; column != WIDTH; column++)
                {
                    Spectrumf pixel_radiance;
                    for (unsigned int sample = 0; sample != NUM_SAMPLES; sample++)
                    {
                        const Ray ray{ camera.GenerateRayWorldSpace(Point2ui{ column, row },
                                                                    Point2f{ rng.NextFloat(), rng.NextFloat() }) };

                        // Intersect Ray with BVH
                        TriangleIntersection intersection;
                        Intervalf interval{ Ray::DefaultInterval() };
                        if (bvh.Intersect(ray, interval, intersection))
                        {
                            const float n_dot_wo{ Clamp(Dot(intersection.normal, -ray.Direction()), 0.f, 1.f) };
                            pixel_radiance += Spectrumf{ n_dot_wo };
                        }
                    }

                    film(row, column) = INV_SAMPLES * pixel_radiance;
                }
            }
        }
        const auto end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Average rendering time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / NUM_TRIALS << " ms\n";

        film.WritePNG("render.png");
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
    }

    return 0;
}