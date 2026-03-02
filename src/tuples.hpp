#pragma once

#include "utils.hpp"

// Tuple

struct Tuple
{
    float x;
    float y;
    float z;
    float w;

    bool IsPoint() const
    {
        return Equal(w, 1.f);
    }

    bool IsVector() const
    {
        return Equal(w, 0.f);
    }

    bool IsEqual(const Tuple &other) const
    {
        return Equal(x, other.x) && Equal(y, other.y) && Equal(z, other.z) && Equal(w, other.w);
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