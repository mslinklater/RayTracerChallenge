#pragma once

#include "color.hpp"
#include <string>
#include <vector>

class Canvas
{
  public:
    Canvas(int width, int height);

    int GetWidth() const
    {
        return width;
    }
    int GetHeight() const
    {
        return height;
    }
    Color GetPixel(int x, int y) const;
    void WritePixel(int x, int y, const Color &color);
    void WriteToPPM(const std::string &filename) const;

  private:
    int width;
    int height;
    std::vector<Color> pixels;
};

bool operator==(const Canvas &lhs, const Canvas &rhs);
