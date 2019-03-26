//
// Created by Simon on 2019-03-26.
//

#include "mesh.hpp"

#define TINYPLY_IMPLEMENTATION
#include "tinyply.hpp"

#include <fstream>
#include <sstream>

namespace Geometry
{

Mesh::Mesh(const std::vector<Vector3>& v, const std::vector<Vector3>& n, const std::vector<unsigned int>& i)
    : vertices{ v }, normals{ n }, indices{ i }
{}

Mesh::Mesh(std::vector<Vector3>&& v, std::vector<Vector3>&& n, std::vector<unsigned int>&& i)
    : vertices{ std::move(v) }, normals{ std::move(n) }, indices{ std::move(i) }
{}

Mesh Mesh::LoadPLY(const std::string& filename, bool load_normals)
{
    using namespace tinyply;

    // Open file
    std::ifstream file{ filename, std::ios::binary };
    if (!file.is_open())
    {
        std::ostringstream error_string;
        error_string << "Could not open file: " << filename;
        throw std::runtime_error(error_string.str());
    }

    // Parse .ply header
    PlyFile ply_file;
    ply_file.parse_header(file);

    // Load data
    const std::shared_ptr<PlyData> vertices{ ply_file.request_properties_from_element("vertex", { "x", "y", "z" }) };

    std::shared_ptr<PlyData> normals;
    if (load_normals)
    {
        normals = ply_file.request_properties_from_element("vertex", { "x", "y", "z" });
    }

    const std::shared_ptr<PlyData> faces{ ply_file.request_properties_from_element("face", { "vertex_indices" }, 3) };

    // Read file
    ply_file.read(file);
}

std::vector<Vector3>
Mesh::ComputeSmoothNormals(const std::vector<Vector3>& vertices, const std::vector<unsigned int>& indices)
{
    std::vector<Vector3> normals(vertices.size());

    for (size_t i = 0; i != indices.size(); i += 3)
    {
        const unsigned int i0 = indices[i];
        const unsigned int i1 = indices[i + 1];
        const unsigned int i2 = indices[i + 2];

        const Vector3& v0{ vertices[i0] };
        const Vector3& v1{ vertices[i1] };
        const Vector3& v2{ vertices[i2] };

        const Vector3 n{ Cross(v1 - v0, v2 - v0) };
        normals[i0] += n;
        normals[i1] += n;
        normals[i2] += n;
    }

    for (Vector3& n : normals)
    {
        NormalizeInPlace(n);
    }

    return normals;
}

} // Geometry namespace
