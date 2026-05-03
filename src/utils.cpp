#include "utils.hpp"
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

std::vector<Group> ObjFileToGroups(const std::string& filename);
{
}
