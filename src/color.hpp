#pragma once

struct Color
{
    float r = 0.f;
    float g = 0.f;
    float b = 0.f;

    Color operator+(const Color &other) const
    {
        return Color{r + other.r, g + other.g, b + other.b};
    }

    Color operator-(const Color &other) const
    {
        return Color{r - other.r, g - other.g, b - other.b};
    }

    Color operator*(float scalar) const
    {
        return Color{r * scalar, g * scalar, b * scalar};
    }

    Color operator*(const Color &other) const
    {
        return Color{r * other.r, g * other.g, b * other.b};
    }

    bool operator==(const Color &other) const
    {
        return r == other.r && g == other.g && b == other.b;
    }
};
