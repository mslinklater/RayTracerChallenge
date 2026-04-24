#include "shapes/sphere.hpp"
#include "ray.hpp"
#include "types.hpp"
#include <cassert>
#include <cmath>

bool operator==(const Sphere& s1, const Sphere& s2)
{
    return s1.GetTransform() == s2.GetTransform() && s1.GetMaterial() == s2.GetMaterial() &&
           s1.GetName() == s2.GetName();
}

std::vector<Intersection> Sphere::IntersectLocal(const Ray& ray) const
{
    assert(ray.IsValid());

    // For a sphere centered at the origin with radius 1, the intersection can be calculated using the quadratic
    // formula. The coefficients of the quadratic equation are derived from substituting the ray equation into the
    // sphere equation.

    const Tuple sphereToRay =
        ray.GetOrigin() -
        Point(0.f, 0.f, 0.f); // Since the sphere is at the origin, we can use the ray's origin directly
    const float a = ray.GetDirection() | ray.GetDirection();  // Dot product of direction with itself
    const float b = 2.f * (ray.GetDirection() | sphereToRay); // 2 times the dot product of direction and origin
    const float c = (sphereToRay | sphereToRay) - 1.f;        // Dot product of origin with itself minus radius squared

    const float discriminant = b * b - 4.f * a * c;

    std::vector<Intersection> intersections;
    if (discriminant < 0.f)
    {
        // No intersections
        return intersections;
    }
    else
    {
        const float sqrtDiscriminant = std::sqrt(discriminant);
        const Intersection t1 = Intersection((-b - sqrtDiscriminant) / (2.f * a), worldObjectId);
        const Intersection t2 = Intersection((-b + sqrtDiscriminant) / (2.f * a), worldObjectId);
        intersections.push_back(t1);
        intersections.push_back(t2);
        return intersections;
    }
}

Tuple Sphere::NormalAtLocal(const Tuple& point) const
{
    assert(point.IsValid());

    return point - Point(0.f, 0.f, 0.f); // Normal is the vector from the center of the sphere to the point
}
