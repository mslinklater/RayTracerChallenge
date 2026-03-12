#pragma once
#include "sphere.hpp"
#include "utils.hpp"
#include <initializer_list>

class Intersection
{
public:
    Intersection(float t, const Sphere *object) : t(t), object(object) {}

    float GetT() const { return t; }
    const Sphere *GetObject() const { return object; }

    bool operator==(const Intersection &other) const
    {
        return AreEqual(t, other.t) && object == other.object;
    }

private:
    float t;              // The distance along the ray to the intersection point
    const Sphere *object; // Pointer to the intersected object (in this case, a sphere)
};

// input is a list of intersections, output is the hit (the intersection with the lowest non-negative t)
inline std::vector<Intersection> Intersections(std::initializer_list<Intersection> list)
{
    return std::vector<Intersection>(list);
}

inline Intersection GetClosestIntersection(const std::vector<Intersection> &intersections)
{
    Intersection hit(0.f, nullptr);
    for (const auto &intersection : intersections)
    {
        if (intersection.GetT() >= 0.f)
        {
            if (hit.GetObject() == nullptr || intersection.GetT() < hit.GetT())
            {
                hit = intersection;
            }
        }
    }
    return hit;
}
