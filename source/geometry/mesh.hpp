//
// Created by Simon on 2019-03-26.
//

#ifndef RABBIT2_MESH_HPP
#define RABBIT2_MESH_HPP

#include "geometry/bbox.hpp"

#include <vector>

namespace Geometry
{

class Mesh
{
public:
    Mesh(const std::vector<Vector3>& v, const std::vector<Vector3>& n, const std::vector<unsigned int>& i);

    Mesh(std::vector<Vector3>&& v, std::vector<Vector3>&& n, std::vector<unsigned int>&& i);

    // Load mesh from .obj
    static Mesh LoadOBJ(const std::string& filename);

private:
    // Compute smooth normals for a set of vertices and indices
    static std::vector<Vector3> SmoothNormals(const std::vector<Vector3>& vertices,
                                              const std::vector<unsigned int>& indices);

    // Mesh representation
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<unsigned int> indices;
};

} // Geometry namespace

#endif //RABBIT2_MESH_HPP
