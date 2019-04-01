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
        source/geometry/mesh.cpp
        source/geometry/mesh.hpp
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
        source/geometry/geometry.hpp)

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
