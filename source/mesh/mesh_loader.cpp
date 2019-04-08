//
// Created by simon on 08/04/2019.
//

#include "mesh_loader.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobj.hpp"

#define TINYPLY_IMPLEMENTATION
#include "tinyply.hpp"

#include <fstream>
#include <sstream>

namespace Rabbit
{
namespace MeshLoader
{

const Mesh LoadOBJ(const std::string& filename)
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

    // Store vertices
    std::vector<Geometry::Point3f> vertices(attrib.vertices.size() / 3);
    if (std::is_same<float, real_t>::value)
    {
        std::memcpy(vertices.data(), attrib.vertices.data(), attrib.vertices.size() * sizeof(float));
    }
    else
    {
        for (size_t v = 0; v != vertices.size(); v++)
        {
            vertices[v] = Geometry::Point3f{ attrib.vertices[3 * v], attrib.vertices[3 * v + 1],
                                             attrib.vertices[3 * v + 2] };
        }
    }

    // Store indices
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
    std::vector<Geometry::Vector3f> smooth_normals = SmoothNormals(vertices, indices);

    return { std::move(vertices), std::move(smooth_normals), std::move(indices) };
}

const Mesh LoadPLY(const std::string& filename)
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
    std::vector<Geometry::Point3f> vertices(ply_vertices->count);
    std::memcpy(vertices.data(), ply_vertices->buffer.get(), ply_vertices->buffer.size_bytes());
    std::vector<unsigned int> indices(ply_indices->count * 3);
    std::memcpy(indices.data(), ply_indices->buffer.get(), ply_indices->buffer.size_bytes());

    // Now that we have our vertices and indices, we can compute the normals
    std::vector<Geometry::Vector3f> smooth_normals = SmoothNormals(vertices, indices);

    return { std::move(vertices), std::move(smooth_normals), std::move(indices) };
}

std::vector<Geometry::Vector3f> SmoothNormals(const std::vector<Geometry::Point3f>& vertices,
                                              const std::vector<unsigned int>& indices)
{
    std::vector<Geometry::Vector3f> normals(vertices.size(), Geometry::Vector3f{ 0.f });

    for (size_t f = 0; f != indices.size(); f += 3)
    {
        const unsigned int i0{ indices[f] };
        const unsigned int i1{ indices[f + 1] };
        const unsigned int i2{ indices[f + 2] };

        const Geometry::Point3f v0{ vertices[i0] };
        const Geometry::Point3f v1{ vertices[i1] };
        const Geometry::Point3f v2{ vertices[i2] };

        // We avoid normalisation here to give a weight to the normal proportional to the triangle size
        const Geometry::Vector3f normal{ Cross(v1 - v0, v2 - v0) };

        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;
    }

    for (Geometry::Vector3f& n : normals)
    {
        Geometry::NormalizeInPlace(n);
    }

    return normals;
}

} // MeshLoader namespace

const Mesh LoadMesh(const std::string& filename)
{
    // Check file extension
    const std::size_t extension_position{ filename.find_last_of('.') };
    if (extension_position == std::string::npos)
    {
        std::ostringstream error_message;
        error_message << "Could not determine file extension for " << filename << "\n";
        throw std::runtime_error(error_message.str());
    }
    const std::string extension{ filename.substr(extension_position) };

    // Forward method based on extension
    if (extension == ".ply")
    {
        return MeshLoader::LoadPLY(filename);
    }
    else if (extension == ".obj")
    {
        return MeshLoader::LoadOBJ(filename);
    }
    else
    {
        std::ostringstream error_message;
        error_message << "Unrecognized extension " << extension << "\n";
        throw std::runtime_error(error_message.str());
    }
}

} // Rabbit namespace
