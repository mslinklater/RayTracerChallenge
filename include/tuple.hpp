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

    Tuple operator-(const Tuple &other) const
    {
        return Tuple{x - other.x, y - other.y, z - other.z, w - other.w};
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
        if (scalar == 0.f)
        {
            throw std::invalid_argument("Cannot divide by zero.");
        }
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

    bool operator==(const Tuple &other) const
    {
        return IsEqual(other);
    }

    Tuple Normalize() const;
};

// Point

inline Tuple Point(float x, float y, float z)
{
    return Tuple{x, y, z, 1.f};
}

// Vector

inline Tuple Vector(float x, float y, float z)
{
    return Tuple{x, y, z, 0.f};
}