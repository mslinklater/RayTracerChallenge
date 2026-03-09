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

Tuple Tuple::Reflect(const Tuple &normal) const
{
    // Reflection formula: v - n * 2 * (v . n)
    return *this - normal * 2.f * (*this | normal);
}
