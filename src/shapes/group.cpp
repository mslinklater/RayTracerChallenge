#include "shapes/group.hpp"

std::vector<Intersection> Group::IntersectLocal(const Ray& ray) const
{
    std::vector<Intersection> intersections;
    // agregate intersections from all children
    for (Shape* child : children)
    {
        std::vector<Intersection> childIntersections = child->Intersect(ray);
        intersections.insert(intersections.end(), childIntersections.begin(), childIntersections.end());
    }
    // sort intersections by t value
    std::sort(intersections.begin(), intersections.end(),
              [](const Intersection& a, const Intersection& b) { return a.GetT() < b.GetT(); });
    return intersections;
}

Tuple Group::NormalAtLocal(const Tuple& point) const
{
    return Vector(0.f, 0.f, 0.f);
}
