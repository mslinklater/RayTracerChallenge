#include "shapes/group.hpp"

std::vector<Intersection> Group::IntersectLocal(const Ray& ray) const
{
    std::vector<Intersection> intersections;
    return intersections;
}

Tuple Group::NormalAtLocal(const Tuple& point) const
{
    return Vector(0.f, 0.f, 0.f);
}
