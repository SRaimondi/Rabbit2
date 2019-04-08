//
// Created by simon on 08/04/2019.
//

#ifndef RABBIT2_MESH_LOADER_HPP
#define RABBIT2_MESH_LOADER_HPP

#include "mesh.hpp"

namespace Rabbit
{
namespace MeshLoader
{

const Mesh LoadOBJ(const std::string& filename);

const Mesh LoadPLY(const std::string& filename);

std::vector<Geometry::Vector3f> SmoothNormals(const std::vector<Geometry::Point3f>& vertices,
                                              const std::vector<unsigned int>& indices);

} // MeshLoader namespace

// Load mesh from file
const Mesh LoadMesh(const std::string& filename);

} // Rabbit namespace

#endif //RABBIT2_MESH_LOADER_HPP
