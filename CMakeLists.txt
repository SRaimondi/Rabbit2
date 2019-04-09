cmake_minimum_required(VERSION 3.13)
project(Rabbit2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(source external)

add_executable(Rabbit2
        source/main.cpp
        #source/opencl/error.cpp
        #source/opencl/error.hpp
        source/io/file_io.cpp
        source/io/file_io.hpp
        external/tinyply.hpp
        source/mesh/mesh.cpp
        source/mesh/mesh.hpp
        external/tinyobj.hpp
        source/geometry/bbox.hpp
        source/bvh/bvh.cpp
        source/bvh/bvh.hpp
        source/geometry/ray.hpp
        source/geometry/intersection.hpp
        external/stb_image_write.hpp
        source/camera/camera.cpp
        source/camera/camera.hpp
        source/geometry/common.hpp
        source/geometry/geometry.hpp
        source/geometry/matrix.hpp
        source/geometry/transform.cpp
        source/geometry/transform.hpp
        source/film/film.cpp
        source/film/film.hpp
        source/film/spectrum.hpp
        source/utilities/utilities.hpp
        #source/kdtree/kdtree.cpp
        #source/kdtree/kdtree.hpp
        source/utilities/memory.hpp
        source/sampling/pcg32.hpp
        source/geometry/interval.hpp
        source/mesh/mesh_loader.cpp
        source/mesh/mesh_loader.hpp
        source/sampling/montecarlo.hpp
        source/texture/texture.hpp
        source/texture/constant_texture.hpp
        source/material/material.cpp
        source/material/material.hpp
        source/material/diffuse_material.cpp
        source/material/diffuse_material.hpp
        source/material/emitting_material.cpp
        source/material/emitting_material.hpp
        source/integrator/image_integrator.cpp
        source/integrator/image_integrator.hpp
        source/scene/scene.cpp
        source/scene/scene.hpp
        source/integrator/ray_integrator.hpp
        source/light/light.hpp
        source/light/light.cpp source/integrator/debug_integrator.cpp source/integrator/debug_integrator.hpp)

# find_package(OpenCL)

if (APPLE)
    target_compile_definitions(Rabbit2 PRIVATE CL_SILENCE_DEPRECATION)
endif (APPLE)

# if (OpenCL_FOUND)
#     target_link_libraries(Rabbit2 OpenCL::OpenCL)
# else ()
#     message(FATAL_ERROR "OpenCL not found")
# endif ()

# Specify flags for build
IF (CMAKE_BUILD_TYPE MATCHES Debug)
    TARGET_COMPILE_OPTIONS(Rabbit2 PRIVATE -Wall -Wextra)
ELSEIF (CMAKE_BUILD_TYPE MATCHES Release)
    TARGET_COMPILE_OPTIONS(Rabbit2 PRIVATE -march=native)
ENDIF ()
