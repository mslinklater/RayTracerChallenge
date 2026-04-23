#include "canvas.hpp"
#include "utils.hpp"
#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>

Canvas::Canvas(int width, int height) : width(width), height(height), pixels(width * height)
{
    assert(width > 0);
    assert(height > 0);
}

int Canvas::GetWidth() const
{
    return width;
}

int Canvas::GetHeight() const
{
    return height;
}

Color Canvas::GetPixel(int x, int y) const
{
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        return Color{0.f, 0.f, 0.f}; // Return default color for out-of-bounds pixels
    }
    return pixels[y * width + x];
}

void Canvas::WritePixel(int x, int y, const Color& color)
{
    assert(color.IsValid());

    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        return; // Ignore out-of-bounds writes
    }
    pixels[y * width + x] = color;
}

void Canvas::WriteToPPM(const std::string& filename) const
{
    assert(!filename.empty());

    std::ofstream ppmFile(filename);
    if (!ppmFile.is_open())
    {
        std::cout << "Unable to open file " << filename << std::endl;
        std::cout << "Current path " << std::filesystem::current_path() << std::endl;
        return;
    }

    ppmFile << "P3\n";
    ppmFile << width << ' ' << height << "\n";
    ppmFile << "255\n";

    std::string line;

    auto WriteValue = [&](int value) {
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

bool operator==(const Canvas& lhs, const Canvas& rhs)
{
    if (lhs.GetWidth() != rhs.GetWidth() || lhs.GetHeight() != rhs.GetHeight())
        return false;
    for (int y = 0; y < lhs.GetHeight(); ++y)
    {
        for (int x = 0; x < lhs.GetWidth(); ++x)
        {
            if (!(lhs.GetPixel(x, y) == rhs.GetPixel(x, y)))
                return false;
        }
    }
    return true;
}
