cmake_minimum_required(VERSION 3.14)
project(Rabbit2)

set(CMAKE_CXX_STANDARD 14)

include_directories(source)

add_executable(Rabbit2
        source/main.cpp)

find_package(OpenCL)

if (APPLE)
    target_compile_definitions(Rabbit2 PRIVATE CL_SILENCE_DEPRECATION)
endif (APPLE)

if (NOT OpenCL_FOUND)
    # TODO This is hardocoded to the CUDA folder
    target_include_directories(Rabbit PRIVATE $ENV{CUDA_PATH}/include)
    target_link_libraries(Rabbit PRIVATE $ENV{CUDA_PATH}/lib/x64/OpenCL.lib)
else ()
    target_link_libraries(Rabbit2 OpenCL::OpenCL)
endif ()

# Specify flags for build
IF (CMAKE_BUILD_TYPE MATCHES Debug)
    TARGET_COMPILE_OPTIONS(Rabbit2 PRIVATE -Wall -Wextra)
ENDIF ()