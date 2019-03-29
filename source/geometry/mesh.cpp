//
// Created by Simon on 2019-03-26.
//

#include "mesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobj.hpp"

#define TINYPLY_IMPLEMENTATION
#include "tinyply.hpp"

#include <fstream>
#include <sstream>

namespace Geometry
{

Mesh::Mesh(std::vector<Vector3>&& v, std::vector<Vector3>&& n, std::vector<unsigned int>&& i)
    : vertices{ std::move(v) }, normals{ std::move(n) }, indices{ std::move(i) }
{}

Mesh Mesh::LoadOBJ(const std::string& filename)
{
    using namespace tinyobj;

    attrib_t attrib;
    std::vector<shape_t> shapes;
    std::vector<material_t> materials;

    // Read file
    std::string warn, err;
    const bool tinyobj_result{ LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str()) };

    // Check for errors
    if (!err.empty())
    {
        throw std::runtime_error(err);
    }
    if (!tinyobj_result)
    {
        std::ostringstream error_string;
        error_string << "Error in loading .obj file: " << filename << "\n";
        throw std::runtime_error(error_string.str());
    }

    // Storage for the loaded data
    static_assert(std::is_same<float, real_t>::value, "Tinyobj real_t is not float");
    std::vector<Vector3> vertices(attrib.vertices.size() / 3);
    std::memcpy(vertices.data(), attrib.vertices.data(), attrib.vertices.size() * sizeof(float));

    std::vector<unsigned int> indices;

    // Add all triangles from all shapes to a single list
    size_t index_offset{ 0 };
    for (const auto& shape : shapes)
    {
        // Loop over faces
        for (size_t f = 0; f != shape.mesh.num_face_vertices.size(); f++)
        {
            // Check that the face is a triangle
            if (shape.mesh.num_face_vertices[f] != 3)
            {
                throw std::runtime_error("Face is not a triangle\n");
            }

            // Loop over the vertices in the face
            for (int vertex = 0; vertex != 3; vertex++)
            {
                // Access the vertex data
                const index_t& idx = shape.mesh.indices[index_offset + vertex];
                // Add index of the vertices
                indices.emplace_back(idx.vertex_index);
            }

            // Go to next triangle
            index_offset += 3;
        }
    }

    // Now that we have our vertices and indices, we can compute the normals
    std::vector<Vector3> smooth_normals = SmoothNormals(vertices, indices);

    return { std::move(vertices), std::move(smooth_normals), std::move(indices) };
}

Mesh Mesh::LoadPLY(const std::string& filename)
{
    using namespace tinyply;

    // Open file
    std::ifstream file{ filename, std::ios::binary };
    if (!file.is_open())
    {
        std::ostringstream error_string;
        error_string << "Could not open file: " << filename << "\n";
        throw std::runtime_error(error_string.str());
    }

    // Parse header
    PlyFile ply_file;
    ply_file.parse_header(file);

    // We are only interested int vertices + indices
    const std::shared_ptr<PlyData> ply_vertices{
        ply_file.request_properties_from_element("vertex", { "x", "y", "z" }) };
    const std::shared_ptr<PlyData> ply_indices{
        ply_file.request_properties_from_element("face", { "vertex_indices" }, 3) };

    // Read file
    ply_file.read(file);

    // Allocate space for vertices / indices and copy
    std::vector<Vector3> vertices(ply_vertices->count);
    std::memcpy(vertices.data(), ply_vertices->buffer.get(), ply_vertices->buffer.size_bytes());
    std::vector<unsigned int> indices(ply_indices->count * 3);
    std::memcpy(indices.data(), ply_indices->buffer.get(), ply_indices->buffer.size_bytes());

    // Now that we have our vertices and indices, we can compute the normals
    std::vector<Vector3> smooth_normals = SmoothNormals(vertices, indices);

    return { std::move(vertices), std::move(smooth_normals), std::move(indices) };
}

std::vector<Triangle> Mesh::CreateTriangles() const
{
    std::vector<Triangle> triangles;
    triangles.reserve(indices.size() / 3);
    for (unsigned int index = 0; index != indices.size(); index += 3)
    {
        triangles.emplace_back(index, *this);
    }

    return triangles;
}

std::vector<Vector3> Mesh::SmoothNormals(const std::vector<Vector3>& vertices, const std::vector<unsigned int>& indices)
{
    std::vector<Vector3> normals(vertices.size(), Vector3{ 0.f });

    for (size_t f = 0; f != indices.size(); f += 3)
    {
        const unsigned int i0{ indices[f] };
        const unsigned int i1{ indices[f + 1] };
        const unsigned int i2{ indices[f + 2] };

        const Vector3 v0{ vertices[i0] };
        const Vector3 v1{ vertices[i1] };
        const Vector3 v2{ vertices[i2] };

        // We avoid normalisation here to give a weight to the normal proportional to the triangle size
        const Vector3 normal{ Cross(v1 - v0, v2 - v0) };

        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;
    }

    for (Vector3& n : normals)
    {
        NormalizeInPlace(n);
    }

    return normals;
}

std::ostream& operator<<(std::ostream& os, const Mesh& mesh)
{
    os << "Vertices: " << mesh.vertices.size() << "\n";
    os << "Triangles: " << mesh.indices.size() / 3 << "\n";

    return os;
}

Triangle::Triangle(unsigned int fi, const Mesh& m) noexcept
    : first_index{ fi }, mesh{ m }
{}

} // Geometry namespace
