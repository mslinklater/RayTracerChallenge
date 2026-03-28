#include "plane.hpp"

Tuple Plane::NormalAtLocal(const Tuple &point) const
{
    return Vector(0.f, 1.f, 0.f); // The normal of a plane is constant everywhere and points up the y-axis
}

std::vector<float> Plane::IntersectLocal(const Ray &ray) const
{
    // A plane is infinite, so it only intersects with a ray if the ray is not parallel to the plane
    if (std::abs(ray.GetDirection().y) < 1e-6)
    {
        return {};
    }

    // float t = -ray.origin.y / ray.direction.y;
    // return {t};
    return {};
}