#include "tuple.hpp"

Tuple Tuple::Normalize() const
{
    float mag = Magnitude();
    if (mag == 0.f)
    {
        return *this; // Can't normalize a zero vector, return it as is
    }
    return Tuple{x / mag, y / mag, z / mag, w};
}