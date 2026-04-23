#include "intersection.hpp"
#include "maths.hpp"
#include <cassert>
#include <cmath>

bool Intersection::operator==(const Intersection& other) const
{
    assert(std::isfinite(t));
    assert(std::isfinite(other.t));

    return AreEqual(t, other.t) && objectId == other.objectId;
}

Intersection::Intersection(float t, ObjectId objectId) : t(t), objectId(objectId)
{
    assert(std::isfinite(t));
}

float Intersection::GetT() const
{
    return t;
}

ObjectId Intersection::GetObjectId() const
{
    return objectId;
}
