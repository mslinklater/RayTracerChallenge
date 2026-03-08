#pragma once
#include "sphere.hpp"
#include "utils.hpp"
#include <initializer_list>

struct Intersection
{
    Intersection(float t, const Sphere *object) : t(t), object(object) {}

    float t;              // The distance along the ray to the intersection point
    const Sphere *object; // Pointer to the intersected object (in this case, a sphere)

    bool operator==(const Intersection &other) const
    {
        return AreEqual(t, other.t) && object == other.object;
    }
};

// input is a list of intersections, output is the hit (the intersection with the lowest non-negative t)
inline std::vector<Intersection> Intersections(std::initializer_list<Intersection> list)
{
    return std::vector<Intersection>(list);
}

inline Intersection Hit(const std::vector<Intersection> &intersections)
{
    Intersection hit(0.f, nullptr);
    for (const auto &intersection : intersections)
    {
        if (intersection.t >= 0.f)
        {
            if (hit.object == nullptr || intersection.t < hit.t)
            {
                hit = intersection;
            }
        }
    }
    return hit;
}