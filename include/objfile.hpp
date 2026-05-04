#pragma once
#include <string>
#include <vector>

//--------------------------------------------------------

struct ObjFileVertex
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

//--------------------------------------------------------

static constexpr uint32_t kInvalidVertexIndex = 0;

struct ObjFileTriangle
{
    const ObjFileVertex* p1 = nullptr;
    const ObjFileVertex* p2 = nullptr;
    const ObjFileVertex* p3 = nullptr;
};

bool operator==(const ObjFileVertex& lhs, const ObjFileVertex& rhs);

//--------------------------------------------------------

struct ObjFileFace
{
    std::vector<uint32_t> vertexIndices;
};

//--------------------------------------------------------

struct ObjFileGroup
{
    std::string name;
    std::vector<ObjFileFace> faces;
    std::vector<ObjFileTriangle> triangles;
};

//--------------------------------------------------------

class ObjFile
{
  public:
    struct GroupIndex
    {
        uint32_t value = 0;
    };

    ObjFile(const std::string& filename);

    uint32_t GetNumGroups() const;
    uint32_t GetNumVertices() const;
    const ObjFileGroup& GetGroup(GroupIndex groupIndex) const;
    const ObjFileVertex& GetVertex(int index) const;
    const ObjFileTriangle& GetTriangle(GroupIndex groupIndex, uint32_t triangleIndex) const;

    GroupIndex GetDefaultGroupIndex() const;
    GroupIndex GetGroupIndex(const std::string& groupName) const;

  private:
    std::optional<ObjFileVertex> ParseVertex(const std::string& line) const;
    std::optional<ObjFileFace> ParseFace(const std::string& line) const;
    std::optional<ObjFileGroup> ParseGroup(const std::string& line);
    void BuildTriangles();

    std::vector<std::shared_ptr<ObjFileGroup>> groups;
    std::shared_ptr<ObjFileGroup> currentGroup; // TODO: Can we get rid of this ?

    std::vector<ObjFileVertex> vertices;
};
