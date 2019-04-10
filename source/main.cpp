#include "mesh/mesh_loader.hpp"
#include "geometry/common.hpp"
#include "sampling/pcg32.hpp"
#include "material/diffuse_material.hpp"
#include "material/emitting_material.hpp"
#include "texture/constant_texture.hpp"
#include "integrator/image_integrator.hpp"
#include "integrator/debug_integrator.hpp"
#include "camera/perspective_camera.hpp"
#include "camera/orthographic_camera.hpp"

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
        const Mesh cornell_box{ LoadMesh("../models/cornell/cornell_box.ply") };
        const Mesh cornell_cube{ LoadMesh("../models/cornell/cornell_cube.ply") };
        const Mesh cornell_sphere{ LoadMesh("../models/cornell/cornell_sphere.ply") };
        const Mesh cornell_light{ LoadMesh("../models/cornell/cornell_light.ply") };
        const auto mesh_read_end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Read meshes in "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(mesh_read_end - mesh_read_start).count()
                  << " ms\n";

        // Transform
        const auto identity_tr{ std::make_shared<const Transform>() };

        // Materials
        const auto diffuse_material{ std::make_shared<const DiffuseMaterial>(
            std::make_shared<const ConstantTexture<const Spectrumf>>(Spectrumf{ 1.f })) };
        const auto emitting_material{ std::make_shared<const EmittingMaterial>(
            std::make_shared<const ConstantTexture<const Spectrumf>>(Spectrumf{ 10.f })) };


        std::vector<Triangle> scene_triangles;
        cornell_box.CreateTriangles(identity_tr, diffuse_material, scene_triangles);
        cornell_cube.CreateTriangles(identity_tr, diffuse_material, scene_triangles);
        cornell_sphere.CreateTriangles(identity_tr, diffuse_material, scene_triangles);
        cornell_light.CreateTriangles(identity_tr, emitting_material, scene_triangles);

        // Create BVH
        const auto bvh_start{ std::chrono::high_resolution_clock::now() };
        BVH bvh{ BVHConfig{ 4, 1.f, 0.2f, 128 }, scene_triangles };
        const auto bvh_end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Built BVH in "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(bvh_end - bvh_start).count() << " ms\n";

        // Create scene
        const Scene scene{ std::move(bvh) };

        constexpr unsigned int WIDTH{ 1080 };
        constexpr unsigned int HEIGHT{ 1080 };
        constexpr unsigned int NUM_SAMPLES{ 128 };

        Film perspective_film{ WIDTH, HEIGHT };

        // Create cameras
        const PerspectiveCamera perspective_camera{ Point3f{ 0.f, 0.f, 30.f }, Point3f{}, Vector3f{ 0.f, 1.f, 0.f },
                                                    60.f, WIDTH, HEIGHT };

        // Create integrator
        const ImageIntegrator image_integrator{ std::make_unique<const DebugIntegrator>(DebugMode::NORMAL),
                                                Geometry::Point2ui{ 16, 16 }, NUM_SAMPLES };

        const auto start{ std::chrono::high_resolution_clock::now() };
        image_integrator.RenderImage(scene, perspective_camera, perspective_film);
        const auto end{ std::chrono::high_resolution_clock::now() };

        // Write out image
        perspective_film.WritePNG("render_perspective.png");
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
    }

    return 0;
}