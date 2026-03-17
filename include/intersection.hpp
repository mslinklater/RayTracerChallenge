#pragma once
#include "sphere.hpp"
#include "ray.hpp"
#include <initializer_list>

struct Computations
{
    float t = 0.f;
    const Sphere *object = nullptr;
    Tuple point = {0.f, 0.f, 0.f, 1.f};
    Tuple eyeVector = {0.f, 0.f, 0.f, 0.f};
    Tuple normalVector = {0.f, 0.f, 0.f, 0.f};
    bool inside = false; // Indicates whether the intersection occurs on the inside of the object
};

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
