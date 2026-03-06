#include "utils.hpp"
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

bool AreEqual(float a, float b)
{
    return std::abs(a - b) < 0.00001f;
}

int ToPPMChannel(float value)
{
    const float clamped = std::clamp(value, 0.0f, 1.0f);
    return static_cast<int>(std::round(clamped * 255.0f));
}

std::vector<std::string> ReadLinesFromFile(const std::string &filename)
{
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
