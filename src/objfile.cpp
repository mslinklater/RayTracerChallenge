#include "objfile.hpp"
#include "filesystem.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

//--------------------------------------------------------
// ObjFileVertex
//--------------------------------------------------------

bool operator==(const ObjFileVertex& lhs, const ObjFileVertex& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

//--------------------------------------------------------
// ObjFileVertexNormal
//--------------------------------------------------------

bool operator==(const ObjFileVertexNormal& lhs, const ObjFileVertexNormal& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

//--------------------------------------------------------
// ObjFileTriangle
//--------------------------------------------------------

bool operator==(const ObjFileTriangle& lhs, const ObjFileTriangle& rhs)
{
    return *lhs.p1 == *rhs.p1 && *lhs.p2 == *rhs.p2 && *lhs.p3 == *rhs.p3 &&
           ((lhs.n1 == nullptr && rhs.n1 == nullptr) ||
            (lhs.n1 != nullptr && rhs.n1 != nullptr && *lhs.n1 == *rhs.n1)) &&
           ((lhs.n2 == nullptr && rhs.n2 == nullptr) ||
            (lhs.n2 != nullptr && rhs.n2 != nullptr && *lhs.n2 == *rhs.n2)) &&
           ((lhs.n3 == nullptr && rhs.n3 == nullptr) || (lhs.n3 != nullptr && rhs.n3 != nullptr && *lhs.n3 == *rhs.n3));
}

//--------------------------------------------------------
// ObjFile
//--------------------------------------------------------

ObjFile::ObjFile(const std::string& filename)
{
    // Find the input file and open it for reading
    std::filesystem::path path = Filesystem::FindFile(filename); // throws an exception if the file cannot be found

    if (path.empty())
    {
        throw std::runtime_error("File not found: " + filename);
    }
    std::ifstream file(path);

    // Setup the groups
    currentGroup = std::make_shared<ObjFileGroup>();
    currentGroup->name = "default";
    groups.push_back(currentGroup);

    // Parse the file line by line, building up the vertex and face lists
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

        auto vertexNormal = ParseVertexNormal(line);
        if (vertexNormal)
        {
            vertexNormals.emplace_back(vertexNormal->x, vertexNormal->y, vertexNormal->z);
            continue;
        }

        auto face = ParseFace(line);
        if (face)
        {
            currentGroup->faces.push_back(std::move(*face));
            continue;
        }

        auto group = ParseGroup(line);
        if (group)
        {
            groups.push_back(std::make_shared<ObjFileGroup>(*group));
            currentGroup = groups.back();
            continue;
        }
    }

    // File is now parsed
    // Now we can build the triangle list

    BuildTriangles();
}

//--------------------------------------------------------

uint32_t ObjFile::GetNumGroups() const
{
    return static_cast<uint32_t>(groups.size());
}

//--------------------------------------------------------

const ObjFileGroup& ObjFile::GetGroup(ObjFile::GroupIndex groupIndex) const
{
    if (groupIndex.value >= groups.size())
    {
        throw std::out_of_range("Invalid group index: " + std::to_string(groupIndex.value));
    }

    return *groups[groupIndex.value];
}

//--------------------------------------------------------

const ObjFileTriangle& ObjFile::GetTriangle(ObjFile::GroupIndex groupIndex, uint32_t triangleIndex) const
{
    const ObjFileGroup& group = GetGroup(groupIndex);

    if ((triangleIndex < 1) || (triangleIndex > group.triangles.size()))
    {
        throw std::out_of_range("Triangle index out of range: " + std::to_string(triangleIndex));
    }
    return group.triangles[triangleIndex - 1];
}

//--------------------------------------------------------

uint32_t ObjFile::GetNumVertices() const
{
    return vertices.size();
}

//--------------------------------------------------------

uint32_t ObjFile::GetNumVertexNormals() const
{
    return vertexNormals.size();
}

//--------------------------------------------------------

const ObjFileVertex& ObjFile::GetVertex(int index) const
{
    if (index < 1 || index > vertices.size())
    {
        throw std::out_of_range("Vertex index out of range: " + std::to_string(index));
    }
    return vertices[index - 1];
}

//--------------------------------------------------------

const ObjFileVertexNormal& ObjFile::GetVertexNormal(int index) const
{
    if (index < 1 || index > vertexNormals.size())
    {
        throw std::out_of_range("Vertex normal index out of range: " + std::to_string(index));
    }
    return vertexNormals[index - 1];
}

//--------------------------------------------------------

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

//--------------------------------------------------------

std::optional<ObjFileVertexNormal> ObjFile::ParseVertexNormal(const std::string& line) const
{
    std::istringstream stream(line);
    std::string prefix;
    ObjFileVertexNormal vertexNormal;

    if (!(stream >> prefix >> vertexNormal.x >> vertexNormal.y >> vertexNormal.z) || prefix != "vn")
    {
        return std::nullopt;
    }

    stream >> std::ws; // consume any trailing whitespace
    if (!stream.eof())
    {
        return std::nullopt; // extra data after the vertex coordinates
    }

    return vertexNormal;
}

//--------------------------------------------------------

std::optional<ObjFileFace> ObjFile::ParseFace(const std::string& line) const
{
    // Parses an index from a string. Returns an optionsl
    auto parseIndex = [](const std::string& token) -> std::optional<uint32_t> {
        if (token.empty())
        {
            return std::nullopt;
        }

        std::istringstream tokenStream(token);
        uint32_t index = 0;
        tokenStream >> index;
        if (tokenStream.fail() || index == 0)
        {
            return std::nullopt;
        }

        tokenStream >> std::ws;
        if (!tokenStream.eof())
        {
            return std::nullopt;
        }

        return index;
    };

    // Parses a pair from a string... a required vertex index and an optional normal index. The returned pair is itself
    // optional
    auto parseFaceVertex =
        [&](const std::string& token) -> std::optional<std::pair<uint32_t, std::optional<uint32_t>>> {
        std::vector<std::string> parts;
        std::istringstream tokenStream(token);

        for (std::string part; std::getline(tokenStream, part, '/');)
        {
            parts.push_back(part);
        }

        if (parts.empty() || parts.size() > 3)
        {
            return std::nullopt;
        }

        std::optional<uint32_t> vertexIndex = parseIndex(parts[0]);
        if (!vertexIndex)
        {
            return std::nullopt;
        }

        std::optional<uint32_t> normalIndex;
        if (parts.size() == 3 && !parts[2].empty())
        {
            normalIndex = parseIndex(parts[2]);
            if (!normalIndex)
            {
                return std::nullopt;
            }
        }

        return std::pair<uint32_t, std::optional<uint32_t>>{*vertexIndex, normalIndex};
    };

    // Entry point and setup here
    std::istringstream stream(line);
    char prefix;

    std::vector<uint32_t> vertexIndices;
    std::vector<uint32_t> normalIndices;

    bool hasAnyNormals = false;
    bool hasAnyVerticesWithoutNormals = false;

    // If line does not start with 'f' return null
    if (!(stream >> prefix) || prefix != 'f')
    {
        return std::nullopt;
    }

    for (std::string token; stream >> token;)
    {
        std::optional<std::pair<uint32_t, std::optional<uint32_t>>> parsedToken = parseFaceVertex(token);
        if (!parsedToken)
        {
            return std::nullopt;
        }

        vertexIndices.push_back(parsedToken->first);
        if (parsedToken->second)
        {
            hasAnyNormals = true;
            normalIndices.push_back(*parsedToken->second);
        }
        else
        {
            hasAnyVerticesWithoutNormals = true;
        }
    }

    stream >> std::ws;
    if (!stream.eof() || vertexIndices.size() < 3 || (hasAnyNormals && hasAnyVerticesWithoutNormals))
    {
        return std::nullopt;
    }

    ObjFileFace face;
    face.vertexIndices = std::move(vertexIndices);
    face.normalIndices = std::move(normalIndices);

    return face;
}

//--------------------------------------------------------

std::optional<ObjFileGroup> ObjFile::ParseGroup(const std::string& line)
{
    std::istringstream stream(line);
    GroupIndex groupIndex;
    char prefix;
    std::string groupName;

    if (!(stream >> prefix >> groupName) || prefix != 'g')
    {
        return std::nullopt;
    }
    stream >> std::ws; // consume any trailing whitespace
    if (!stream.eof())
    {
        return std::nullopt; // extra data after the vertex coordinates
    }

    ObjFileGroup group;
    group.name = std::move(groupName);
    return group;
}

//--------------------------------------------------------

ObjFile::GroupIndex ObjFile::GetDefaultGroupIndex() const
{
    return GroupIndex{0};
}

//--------------------------------------------------------

ObjFile::GroupIndex ObjFile::GetGroupIndex(const std::string& groupName) const
{
    for (size_t i = 0; i < groups.size(); ++i)
    {
        if (groups[i]->name == groupName)
        {
            return GroupIndex{static_cast<uint32_t>(i)};
        }
    }
    throw std::out_of_range("Group not found: " + groupName);
}

//--------------------------------------------------------

void ObjFile::BuildTriangles()
{
    for (std::shared_ptr<ObjFileGroup>& group : groups)
    {
        for (const ObjFileFace& face : group->faces)
        {
            const bool hasVertexNormals = !face.normalIndices.empty();

            for (size_t i = 1; i < face.vertexIndices.size() - 1; ++i)
            {
                ObjFileTriangle triangle;
                triangle.p1 = &GetVertex(face.vertexIndices[0]);
                triangle.p2 = &GetVertex(face.vertexIndices[i]);
                triangle.p3 = &GetVertex(face.vertexIndices[i + 1]);

                if (hasVertexNormals)
                {
                    triangle.n1 = &GetVertexNormal(face.normalIndices[0]);
                    triangle.n2 = &GetVertexNormal(face.normalIndices[i]);
                    triangle.n3 = &GetVertexNormal(face.normalIndices[i + 1]);
                }

                group->triangles.push_back(triangle);
            }
        }
    }
}
