#include "shapes/cylinder.hpp"
#include "ray.hpp"
#include <cassert>
#include <cmath>

bool operator==(const Cylinder &s1, const Cylinder &s2)
{
    return s1.GetTransform() == s2.GetTransform() && s1.GetMaterial() == s2.GetMaterial() &&
           s1.GetName() == s2.GetName();
}

std::vector<float> Cylinder::IntersectLocal(const Ray &ray) const
{
    assert(ray.IsValid());

    std::vector<float> intersections;

    const float a = ray.GetDirection().x * ray.GetDirection().x + ray.GetDirection().z * ray.GetDirection().z;
    if (std::abs(a) < 1e-6f)
    {
        // Ray is parallel to the y-axis, so it cannot intersect the cylinder's curved surface
        return intersections;
    }
    const float b = (2.f * ray.GetOrigin().x * ray.GetDirection().x) + (2.f * ray.GetOrigin().z * ray.GetDirection().z);
    const float c = ray.GetOrigin().x * ray.GetOrigin().x + ray.GetOrigin().z * ray.GetOrigin().z - 1.f;
    const float discriminant = b * b - 4.f * a * c;
    if (discriminant < 0.f)
    {
        // No intersections
        return intersections;
    }
    else
    {
        const float sqrtDiscriminant = std::sqrt(discriminant);
        const float t0 = (-b - sqrtDiscriminant) / (2.f * a);
        const float t1 = (-b + sqrtDiscriminant) / (2.f * a);

        float y0 = ray.GetOrigin().y + t0 * ray.GetDirection().y;
        if (minimum < y0 && y0 < maximum)
        {
            intersections.push_back(t0);
        }

        float y1 = ray.GetOrigin().y + t1 * ray.GetDirection().y;
        if (minimum < y1 && y1 < maximum)
        {
            intersections.push_back(t1);
        }

        return intersections;
    }
}

Tuple Cylinder::NormalAtLocal(const Tuple &point) const
{
    assert(point.IsValid());

    return Vector(point.x, 0.f, point.z);
}
