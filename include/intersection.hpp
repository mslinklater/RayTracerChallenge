#pragma once
#include <cstddef>
#include <initializer_list>

using ObjectId = std::size_t;
static constexpr ObjectId kInvalidObjectId = static_cast<ObjectId>(-1);

class Intersection
{
public:
    Intersection(float t, ObjectId objectId) : t(t), objectId(objectId) {}

    float GetT() const { return t; }
    ObjectId GetObjectId() const { return objectId; }

    bool operator==(const Intersection &other) const;

private:
    float t;           // The distance along the ray to the intersection point
    ObjectId objectId; // Stable id of the intersected object (invalid id means no hit)
};
