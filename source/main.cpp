#include "mesh/mesh_loader.hpp"
#include "geometry/common.hpp"
#include "sampling/pcg32.hpp"
#include "material/diffuse_material.hpp"
#include "material/emitting_material.hpp"
#include "texture/constant_texture.hpp"
#include "integrator/image_integrator.hpp"
#include "integrator/debug_integrator.hpp"
#include "integrator/direct_light_integrator.hpp"
#include "camera/perspective_camera.hpp"
#include "camera/orthographic_camera.hpp"
#include "light/point_light.hpp"

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
        const Mesh cornell_box{ LoadMesh("../models/cornell/cornell_box.ply", true, false) };
        const Mesh cornell_cube{ LoadMesh("../models/cornell/cornell_cube.ply", true, true) };
        const Mesh cornell_sphere{ LoadMesh("../models/cornell/cornell_sphere.ply", false, true) };
        const Mesh cornell_light{ LoadMesh("../models/cornell/cornell_light.ply", true, true) };
        const Mesh cornell_dragon{ LoadMesh("../models/cornell/cornell_dragon.ply", false, false) };
        const auto mesh_read_end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Read meshes in "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(mesh_read_end - mesh_read_start).count()
                  << " ms\n";

        // Transform
        const auto identity_tr{ std::make_shared<const Transform>() };

        // Materials
        const auto diffuse_white_material{ std::make_shared<const DiffuseMaterial>(
            std::make_shared<const ConstantTexture<const Spectrumf>>(Spectrumf{ 1.f })) };
        const auto diffuse_green_material{ std::make_shared<const DiffuseMaterial>(
            std::make_shared<const ConstantTexture<const Spectrumf>>(Spectrumf{ 0.1f, 0.9f, 0.1f })) };
        const auto emitting_material{ std::make_shared<const EmittingMaterial>(
            std::make_shared<const ConstantTexture<const Spectrumf>>(Spectrumf{ 20.f })) };


        std::vector<Triangle> scene_triangles;
        cornell_box.CreateTriangles(identity_tr, diffuse_white_material, scene_triangles);
        cornell_cube.CreateTriangles(identity_tr, diffuse_green_material, scene_triangles);
        cornell_sphere.CreateTriangles(identity_tr, diffuse_white_material, scene_triangles);
        cornell_light.CreateTriangles(identity_tr, emitting_material, scene_triangles);
        cornell_dragon.CreateTriangles(identity_tr, diffuse_white_material, scene_triangles);

        // Create BVH
        const auto bvh_start{ std::chrono::high_resolution_clock::now() };
        BVH bvh{ BVHConfig{ 4, 1.f, 0.2f, 128 }, std::move(scene_triangles) };
        const auto bvh_end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Built BVH in "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(bvh_end - bvh_start).count() << " ms\n";

        // Create scene
        Scene scene{ std::move(bvh) };

        // Add lights
        scene.AddLight(std::make_unique<const PointLight>(Point3f{ 0.f, 0.f, 50.f }, Spectrumf{ 1000.f }));
        scene.SetupAreaLights(5);

        // Create film
        constexpr unsigned int WIDTH{ 512 };
        constexpr unsigned int HEIGHT{ 512 };
        constexpr unsigned int NUM_SAMPLES{ 4 };
        Film film{ WIDTH, HEIGHT };

        // Create cameras
        const PerspectiveCamera perspective_camera{ Point3f{ 0.f, 0.f, 30.f }, Point3f{}, Vector3f{ 0.f, 1.f, 0.f },
                                                    60.f, WIDTH, HEIGHT };

        // Create integrator
        const ImageIntegrator image_integrator{ std::make_unique<const DirectLightIntegrator>(),
                                                Geometry::Point2ui{ 8, 8 }, NUM_SAMPLES };

        const auto start{ std::chrono::high_resolution_clock::now() };
        image_integrator.RenderImage(scene, perspective_camera, film);
        const auto end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Rendering time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " ms\n";

        // Write out image
        film.WritePNG("render.png");
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
    }

    return 0;
}