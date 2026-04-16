#include "intersection.hpp"
#include "maths.hpp"
#include <cassert>
#include <cmath>

bool Intersection::operator==(const Intersection &other) const
{
    assert(std::isfinite(t));
    assert(std::isfinite(other.t));

    return AreEqual(t, other.t) && objectId == other.objectId;
}
