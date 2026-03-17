#include "intersection.hpp"
#include "maths.hpp"

bool Intersection::operator==(const Intersection &other) const
{
    return AreEqual(t, other.t) && object == other.object;
}
