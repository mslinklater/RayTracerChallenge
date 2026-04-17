#include "color.hpp"
#include "maths.hpp"
#include <cassert>
#include <cmath>

namespace
{
bool IsFiniteColor(const Color &color)
{
    return std::isfinite(color.r) && std::isfinite(color.g) && std::isfinite(color.b);
}
} // namespace

Color Color::operator+(const Color &other) const
{
    assert(IsFiniteColor(*this));
    assert(IsFiniteColor(other));
    return Color{r + other.r, g + other.g, b + other.b};
}

Color Color::operator-(const Color &other) const
{
    assert(IsFiniteColor(*this));
    assert(IsFiniteColor(other));
    return Color{r - other.r, g - other.g, b - other.b};
}

Color Color::operator*(const float scalar) const
{
    assert(IsFiniteColor(*this));
    assert(std::isfinite(scalar));
    return Color{r * scalar, g * scalar, b * scalar};
}

Color Color::operator*(const Color &other) const
{
    assert(IsFiniteColor(*this));
    assert(IsFiniteColor(other));
    return Color{r * other.r, g * other.g, b * other.b};
}

bool Color::operator==(const Color &other) const
{
    assert(IsFiniteColor(*this));
    assert(IsFiniteColor(other));
    return AreEqual(r, other.r) && AreEqual(g, other.g) && AreEqual(b, other.b);
}
