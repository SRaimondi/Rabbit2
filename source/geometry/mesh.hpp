//
// Created by Simon on 2019-03-26.
//

#ifndef RABBIT2_MESH_HPP
#define RABBIT2_MESH_HPP

#include "geometry/vector.hpp"

#include <vector>

namespace Geometry
{

class Mesh
{
public:
    Mesh(const std::vector<Vector3>& v, const std::vector<Vector3>& n, const std::vector<unsigned int>& i);

    Mesh(std::vector<Vector3>&& v, std::vector<Vector3>&& n, std::vector<unsigned int>&& i);

    // Load mesh from PLY
    static Mesh LoadPLY(const std::string& filename, bool load_normals = true);

    // Compute smooth normals for a given set of vertices and indices
    static std::vector<Vector3> ComputeSmoothNormals(const std::vector<Vector3>& vertices,
                                                     const std::vector<unsigned int>& indices);

private:
    // Mesh representation
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<unsigned int> indices;
};

} // Geometry namespace

#endif //RABBIT2_MESH_HPP
