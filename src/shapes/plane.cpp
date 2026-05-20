#include "shapes/plane.hpp"

#include <cassert>
#include <cmath>

Tuple Plane::NormalAtLocal(const Tuple& point, const Intersection& intersection) const
{
    assert(point.IsValid());
    return Vector(0.f, 1.f, 0.f); // The normal of a plane is constant everywhere and points up the y-axis
}

std::vector<Intersection> Plane::IntersectLocal(const Ray& ray) const
{
    assert(ray.IsValid());

    std::vector<Intersection> intersections;
    // A plane is infinite, so it only intersects with a ray if the ray is not parallel to the plane
    if (std::abs(ray.GetDirection().y) < 1e-6)
    {
        return intersections;
    }

    const float t = -ray.GetOrigin().y / ray.GetDirection().y;
    intersections.push_back(Intersection(t, GetObjectId()));
    return intersections;
}

BoundingBox Plane::GetBounds() const
{
    // A plane is infinite, so we return an infinite bounding box
    return BoundingBox(Point(-std::numeric_limits<float>::max(), 0.f, -std::numeric_limits<float>::max()),
                       Point(std::numeric_limits<float>::max(), 0.f, std::numeric_limits<float>::max()));
}
