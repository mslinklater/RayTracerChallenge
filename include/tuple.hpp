#pragma once

#include "utils.hpp"

// Tuple

struct Tuple
{
    float x;
    float y;
    float z;
    float w;

    Tuple operator+(const Tuple &other) const;
    Tuple operator-(const Tuple &other) const;
    Tuple operator-() const;
    Tuple operator*(float scalar) const;
    Tuple operator/(float scalar) const;

    // Dot product
    float operator|(const Tuple &other) const;
    Tuple operator^(const Tuple &other) const;

    float Magnitude() const;
    bool IsPoint() const;
    bool IsVector() const;
    bool IsEqual(const Tuple &other) const;
    bool operator==(const Tuple &other) const;

    Tuple Normalize() const;
    Tuple Reflect(const Tuple &normal) const;
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