#include "intersection.hpp"
#include "maths.hpp"
#include <cassert>
#include <cmath>

// Intersection

Intersection::Intersection() : t(0.f), objectId(kInvalidObjectId)
{
}

Intersection::Intersection(float t, ObjectId objectId) : t(t), objectId(objectId)
{
    assert(std::isfinite(t));
}

bool Intersection::operator==(const Intersection& other) const
{
    assert(std::isfinite(t));
    assert(std::isfinite(other.t));

    return AreEqual(t, other.t) && objectId == other.objectId;
}

float Intersection::GetT() const
{
    return t;
}

ObjectId Intersection::GetObjectId() const
{
    return objectId;
}

// IntersectionWithUV

IntersectionWithUV::IntersectionWithUV(float t, ObjectId objectId, float u, float v)
    : Intersection(t, objectId), u(u), v(v)
{
    assert(std::isfinite(t));
}

float IntersectionWithUV::GetU() const
{
    return u;
}

float IntersectionWithUV::GetV() const
{
    return v;
}
