#include "objfile.hpp"
#include "filesystem.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

bool operator==(const ObjFileVertex& lhs, const ObjFileVertex& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

ObjFile::ObjFile(const std::string& filename)
{
    std::filesystem::path path = Filesystem::FindFile(filename); // throws an exception if the file cannot be found

    if (path.empty())
    {
        throw std::runtime_error("File not found: " + filename);
    }
    std::ifstream file(path);

    for (std::string line; std::getline(file, line);)
    {
        if (line.empty())
        {
            continue;
        }
        auto vertex = ParseVertex(line);
        if (vertex)
        {
            vertices.emplace_back(vertex->x, vertex->y, vertex->z);
            continue;
        }
        auto face = ParseFace(line);
        if (face)
        {
            faces.push_back(std::move(*face));
            continue;
        }
    }
    // File is now parsed
    // Now we can build the triangle list
    BuildTriangles();
}

const ObjFileTriangle& ObjFile::GetTriangle(uint32_t groupIndex, uint32_t triangleIndex) const
{
    if (groupIndex != GetDefaultGroup())
    {
        throw std::out_of_range("Invalid group index: " + std::to_string(groupIndex));
    }
    if ((triangleIndex < 1) || (triangleIndex > triangles.size()))
    {
        throw std::out_of_range("Triangle index out of range: " + std::to_string(triangleIndex));
    }
    return triangles[triangleIndex - 1];
}

uint32_t ObjFile::GetNumVertices() const
{
    return vertices.size();
}

const ObjFileVertex& ObjFile::GetVertex(int index) const
{
    if (index < 1 || index > vertices.size())
    {
        throw std::out_of_range("Vertex index out of range: " + std::to_string(index));
    }
    return vertices[index - 1];
}

std::optional<ObjFileVertex> ObjFile::ParseVertex(const std::string& line) const
{
    std::istringstream stream(line);
    char prefix;
    ObjFileVertex vertex;

    if (!(stream >> prefix >> vertex.x >> vertex.y >> vertex.z) || prefix != 'v')
    {
        return std::nullopt;
    }

    stream >> std::ws; // consume any trailing whitespace
    if (!stream.eof())
    {
        return std::nullopt; // extra data after the vertex coordinates
    }

    return vertex;
}

std::optional<ObjFileFace> ObjFile::ParseFace(const std::string& line) const
{
    // parse lines starting with 'f' followed by 3 or more vertex indices (e.g. "f 1 2 3", "f 3 4 5 6 7 8")
    std::istringstream stream(line);
    char prefix;
    std::vector<uint32_t> indices;
    uint32_t index;

    if (!(stream >> prefix) || prefix != 'f')
    {
        return std::nullopt;
    }
    while (stream >> index)
    {
        indices.push_back(index);
    }
    stream >> std::ws; // consume any trailing whitespace
    if (!stream.eof() || indices.size() < 3)
    {
        return std::nullopt; // extra data after the vertex indices
    }

    ObjFileFace face;
    face.vertexIndices = std::move(indices);

    return face;
}

uint32_t ObjFile::GetDefaultGroup() const
{
    return 0;
}

void ObjFile::BuildTriangles()
{
    for (const ObjFileFace& face : faces)
    {
        for (size_t i = 1; i < face.vertexIndices.size() - 1; ++i)
        {
            ObjFileTriangle triangle;
            triangle.p1 = &GetVertex(face.vertexIndices[0]);
            triangle.p2 = &GetVertex(face.vertexIndices[i]);
            triangle.p3 = &GetVertex(face.vertexIndices[i + 1]);
            triangles.push_back(triangle);
        }
    }
}
