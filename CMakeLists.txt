cmake_minimum_required(VERSION 3.13)
project(Rabbit2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(source external)

add_executable(Rabbit2
        source/main.cpp
        #source/opencl/error.cpp
        #source/opencl/error.hpp
        source/io/file_io.cpp source/io/file_io.hpp
        external/tinyply.hpp
        source/mesh/mesh.cpp source/mesh/mesh.hpp
        external/tinyobj.hpp
        source/geometry/bbox.hpp
        source/bvh/bvh.cpp source/bvh/bvh.hpp
        source/geometry/ray.hpp
        source/geometry/intersection.hpp
        external/stb_image_write.hpp
        source/camera/camera.hpp
        source/geometry/common.hpp
        source/geometry/geometry.hpp
        source/geometry/matrix.hpp
        source/geometry/transform.cpp source/geometry/transform.hpp
        source/film/film.cpp source/film/film.hpp
        source/film/spectrum.hpp
        source/utilities/utilities.hpp
        #source/kdtree/kdtree.cpp
        #source/kdtree/kdtree.hpp
        source/utilities/memory.hpp
        source/sampling/pcg32.hpp
        source/geometry/interval.hpp
        source/mesh/mesh_loader.cpp source/mesh/mesh_loader.hpp
        source/sampling/montecarlo.hpp
        source/texture/texture.hpp
        source/texture/constant_texture.hpp
        source/material/material.cpp source/material/material.hpp
        source/material/diffuse_material.cpp source/material/diffuse_material.hpp
        source/material/emitting_material.cpp source/material/emitting_material.hpp
        source/integrator/image_integrator.cpp source/integrator/image_integrator.hpp
        source/scene/scene.cpp source/scene/scene.hpp
        source/integrator/ray_integrator.hpp source/integrator/ray_integrator.cpp
        source/light/light.hpp source/light/light.cpp
        source/integrator/debug_integrator.cpp source/integrator/debug_integrator.hpp
        source/camera/perspective_camera.cpp source/camera/perspective_camera.hpp
        source/camera/orthographic_camera.cpp source/camera/orthographic_camera.hpp
        source/light/point_light.cpp source/light/point_light.hpp
        source/integrator/direct_light_integrator.cpp source/integrator/direct_light_integrator.hpp
        source/geometry/occlusion_test.hpp
        source/geometry/frame.hpp
        source/light/area_light.cpp source/light/area_light.hpp
        source/sampling/sampler.cpp source/sampling/sampler.hpp
        source/light/infinite_light.cpp source/light/infinite_light.hpp
        source/material/mirror_material.cpp source/material/mirror_material.hpp
        source/integrator/path_tracing_integrator.cpp source/integrator/path_tracing_integrator.hpp)

if (APPLE)
    target_compile_definitions(Rabbit2 PRIVATE CL_SILENCE_DEPRECATION)
endif (APPLE)

# Specify flags for build
IF (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(Rabbit2 PRIVATE -Wall -Wextra -Wpedantic)
ELSEIF (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(Rabbit2 PRIVATE -march=native)
ENDIF ()

find_package(Threads REQUIRED)
target_link_libraries(Rabbit2 PRIVATE Threads::Threads)
