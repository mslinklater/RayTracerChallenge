#include "canvas.hpp"
#include "utils.hpp"
#include <fstream>

namespace
{
}

Canvas::Canvas(int width, int height) : width(width), height(height), pixels(width * height) {}

Color Canvas::GetPixel(int x, int y) const
{
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        return Color{0.f, 0.f, 0.f}; // Return default color for out-of-bounds pixels
    }
    return pixels[y * width + x];
}

void Canvas::WritePixel(int x, int y, const Color &color)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        return; // Ignore out-of-bounds writes
    }
    pixels[y * width + x] = color;
}

void Canvas::WriteToPPM(const std::string &filename) const
{
    std::ofstream ppmFile(filename);
    if (!ppmFile.is_open())
    {
        return;
    }

    ppmFile << "P3\n";
    ppmFile << width << ' ' << height << "\n";
    ppmFile << "255\n";

    std::string line;

    auto WriteValue = [&](int value)
    {
        if (line.length() + std::to_string(value).length() + 1 > 70)
        {
            ppmFile << line << "\n";
            line.clear();
        }
        if (!line.empty())
        {
            line += ' ';
        }
        line += std::to_string(value);
    };

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const Color pixel = pixels[y * width + x];
            WriteValue(ToPPMChannel(pixel.r));
            WriteValue(ToPPMChannel(pixel.g));
            WriteValue(ToPPMChannel(pixel.b));
        }
        ppmFile << line << "\n";
        line.clear();
    }
}
