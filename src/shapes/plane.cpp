#include "shapes/plane.hpp"

#include <cassert>
#include <cmath>

Tuple Plane::NormalAtLocal(const Tuple &point) const
{
    assert(point.IsValid());
    return Vector(0.f, 1.f, 0.f); // The normal of a plane is constant everywhere and points up the y-axis
}

std::vector<float> Plane::IntersectLocal(const Ray &ray) const
{
    assert(ray.IsValid());

    // A plane is infinite, so it only intersects with a ray if the ray is not parallel to the plane
    if (std::abs(ray.GetDirection().y) < 1e-6)
    {
        return {};
    }

    const float t = -ray.GetOrigin().y / ray.GetDirection().y;
    return {t};
}
