#pragma once
#include "tuple.hpp"
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

class ObjFile
{
  public:
    ObjFile(const std::string& filename);

    uint32_t GetNumVertices() const;
    const ObjFileVertex& GetVertex(int index) const;
    const ObjFileTriangle& GetTriangle(uint32_t groupIndex, uint32_t triangleIndex) const;

    uint32_t GetDefaultGroup() const;

  private:
    std::optional<ObjFileVertex> ParseVertex(const std::string& line) const;
    std::optional<ObjFileFace> ParseFace(const std::string& line) const;
    void BuildTriangles();

    std::vector<ObjFileVertex> vertices;
    std::vector<ObjFileFace> faces;
    std::vector<ObjFileTriangle> triangles;
};
