#pragma once
#include "sphere.hpp"
#include "ray.hpp"
#include <initializer_list>

class Intersection
{
public:
    Intersection(float t, const Sphere *object) : t(t), object(object) {}

    float GetT() const { return t; }
    const Sphere *GetObject() const { return object; }

    bool operator==(const Intersection &other) const;

private:
    float t;              // The distance along the ray to the intersection point
    const Sphere *object; // Pointer to the intersected object (in this case, a sphere)
};
