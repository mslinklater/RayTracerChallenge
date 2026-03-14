#include "color.hpp"

Color Color::operator+(const Color &other) const
{
    return Color{r + other.r, g + other.g, b + other.b};
}

Color Color::operator-(const Color &other) const
{
    return Color{r - other.r, g - other.g, b - other.b};
}

Color Color::operator*(float scalar) const
{
    return Color{r * scalar, g * scalar, b * scalar};
}

Color Color::operator*(const Color &other) const
{
    return Color{r * other.r, g * other.g, b * other.b};
}

bool Color::operator==(const Color &other) const
{
    return AreEqual(r, other.r) && AreEqual(g, other.g) && AreEqual(b, other.b);
}