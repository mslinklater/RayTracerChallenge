#include "tuple.hpp"
#include "maths.hpp"
#include <cassert>
#include <cmath>
#include <stdexcept>

Tuple Tuple::Normalize() const
{
    assert(IsValid());
    float mag = Magnitude();
    if (mag == 0.f)
    {
        return *this; // Can't normalize a zero vector, return it as is
    }
    return Tuple{x / mag, y / mag, z / mag, w};
}

Tuple Tuple::Reflect(const Tuple &normal) const
{
    assert(IsValid());
    assert(normal.IsValid());
    assert(normal.IsVector());
    // Reflection formula: v - n * 2 * (v . n)
    return *this - normal * 2.f * (*this | normal);
}

Tuple Tuple::operator+(const Tuple &other) const
{
    assert(IsValid());
    assert(other.IsValid());
    return Tuple{x + other.x, y + other.y, z + other.z, w + other.w};
}

Tuple Tuple::operator-(const Tuple &other) const
{
    assert(IsValid());
    assert(other.IsValid());
    return Tuple{x - other.x, y - other.y, z - other.z, w - other.w};
}

Tuple Tuple::operator-() const
{
    assert(IsValid());
    return Tuple{-x, -y, -z, -w};
}

Tuple Tuple::operator*(float scalar) const
{
    assert(IsValid());
    assert(std::isfinite(scalar));
    return Tuple{x * scalar, y * scalar, z * scalar, w * scalar};
}

Tuple Tuple::operator/(float scalar) const
{
    assert(IsValid());
    assert(std::isfinite(scalar));
    if (scalar == 0.f)
    {
        throw std::invalid_argument("Cannot divide by zero.");
    }
    return Tuple{x / scalar, y / scalar, z / scalar, w / scalar};
}

// Dot product
float Tuple::operator|(const Tuple &other) const
{
    assert(IsValid());
    assert(other.IsValid());
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

Tuple Tuple::operator^(const Tuple &other) const
{
    assert(IsValid());
    assert(other.IsValid());
    assert(IsVector());
    assert(other.IsVector());
    return Tuple{
        y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x,
        0.f // Cross product results in a vector, so w is 0
    };
}

float Tuple::Magnitude() const
{
    assert(IsValid());
    return std::sqrt(x * x + y * y + z * z);
}

bool Tuple::IsPoint() const
{
    return w == 1.f;
}

bool Tuple::IsVector() const
{
    return w == 0.f;
}

bool Tuple::IsValid() const
{
    return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w);
}

bool Tuple::IsEqual(const Tuple &other) const
{
    assert(IsValid());
    assert(other.IsValid());
    return AreEqual(x, other.x) && AreEqual(y, other.y) && AreEqual(z, other.z) && AreEqual(w, other.w);
}

bool Tuple::operator==(const Tuple &other) const
{
    return IsEqual(other);
}
