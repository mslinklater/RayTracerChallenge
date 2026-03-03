#pragma once

#include "utils.hpp"

// Tuple

struct Tuple
{
    float x;
    float y;
    float z;
    float w;

    Tuple operator+(const Tuple &other) const
    {
        return Tuple{x + other.x, y + other.y, z + other.z, w + other.w};
    }

    Tuple operator-() const
    {
        return Tuple{-x, -y, -z, -w};
    }

    Tuple operator*(float scalar) const
    {
        return Tuple{x * scalar, y * scalar, z * scalar, w * scalar};
    }

    Tuple operator/(float scalar) const
    {
        return Tuple{x / scalar, y / scalar, z / scalar, w / scalar};
    }

    // Dot product
    float operator|(const Tuple &other) const
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    Tuple operator^(const Tuple &other) const
    {
        return Tuple{
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x,
            0.f // Cross product results in a vector, so w is 0
        };
    }

    float Magnitude() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    bool IsPoint() const
    {
        return w == 1.f;
    }

    bool IsVector() const
    {
        return w == 0.f;
    }

    bool IsEqual(const Tuple &other) const
    {
        return AreEqual(x, other.x) && AreEqual(y, other.y) && AreEqual(z, other.z) && AreEqual(w, other.w);
    }

    Tuple Normalize() const
    {
        float mag = Magnitude();
        if (mag == 0.f)
        {
            return *this; // Can't normalize a zero vector, return it as is
        }
        return Tuple{x / mag, y / mag, z / mag, w};
    }
};

// Point

Tuple Point(float x, float y, float z)
{
    return Tuple{x, y, z, 1.f};
}

// Vector

Tuple Vector(float x, float y, float z)
{
    return Tuple{x, y, z, 0.f};
}