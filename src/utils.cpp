#include "utils.hpp"
#include "objfile.hpp"
#include "shapes/triangle.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

int ToPPMChannel(float value)
{
    assert(std::isfinite(value));
    const float clamped = std::clamp(value, 0.0f, 1.0f);
    return static_cast<int>(std::round(clamped * 255.0f));
}

std::vector<std::string> ReadLinesFromFile(const std::string& filename)
{
    assert(!filename.empty());
    // load 'test.ppm' as a text file
    std::ifstream ppmFile(filename);
    if (!ppmFile.is_open())
    {
        return {};
    }

    // read the file line by line into a vector of strings
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(ppmFile, line))
    {
        lines.push_back(line);
    }

    return lines;
}

Sphere GlassSphere(const std::string& name)
{
    assert(!name.empty());
    Sphere sphere(name);
    Material mat;
    mat.SetTransparency(1.0f);
    mat.SetRefractiveIndex(1.5f);
    sphere.SetMaterial(mat);
    return sphere;
}

std::vector<Group> ObjFileToGroups(const std::string& filename)
{
    assert(!filename.empty());

    ObjFile objFile(filename);
    std::vector<Group> groups;
    groups.reserve(objFile.GetNumGroups());

    for (uint32_t groupIndexValue = 0; groupIndexValue < objFile.GetNumGroups(); ++groupIndexValue)
    {
        const ObjFileGroup& objGroup = objFile.GetGroup(ObjFile::GroupIndex{groupIndexValue});
        if (objGroup.triangles.empty())
        {
            continue;
        }

        Group group(objGroup.name);
        for (const ObjFileTriangle& objTriangle : objGroup.triangles)
        {
            assert(objTriangle.p1 != nullptr);
            assert(objTriangle.p2 != nullptr);
            assert(objTriangle.p3 != nullptr);

            const Tuple p1 = Point(objTriangle.p1->x, objTriangle.p1->y, objTriangle.p1->z);
            const Tuple p2 = Point(objTriangle.p2->x, objTriangle.p2->y, objTriangle.p2->z);
            const Tuple p3 = Point(objTriangle.p3->x, objTriangle.p3->y, objTriangle.p3->z);
            group.AddChild(Triangle(p1, p2, p3));
        }

        groups.push_back(group);
    }

    return groups;
}
