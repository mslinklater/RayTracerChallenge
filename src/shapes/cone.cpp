#include "shapes/cone.hpp"
#include "maths.hpp"

std::vector<float> Cone::IntersectLocal(const Ray &ray) const
{
    // a = (d.x)^2 - (d.y)^2 + (d.z)^2
    // b = 2 * (o.x * d.x - o.y * d.y + o.z * d.z)
    // c = (o.x)^2 - (o.y)^2 + (o.z)^2

    assert(ray.IsValid());
    std::vector<float> intersections;

    const Tuple origin = ray.GetOrigin();
    const Tuple direction = ray.GetDirection();

    float a = direction.x * direction.x - direction.y * direction.y + direction.z * direction.z;
    float b = 2.f * (origin.x * direction.x - origin.y * direction.y + origin.z * direction.z);
    float c = origin.x * origin.x - origin.y * origin.y + origin.z * origin.z;

    if (std::abs(a) < 1e-6f)
    {
        if (std::abs(b) < 1e-6f)
        {
            return intersections;
        }
        intersections.push_back(-c / b);
        return intersections;
    }
    else
    {
        // same algorithm as cylinder, but with different a, b and c
        float discriminant = b * b - 4.f * a * c;

        // discriminants which should be zero can be slightly negative due to floating point errors, so we need to check
        // for that
        if ((discriminant < 0.f) && !AreEqual(discriminant, 0.f))
        {
            return intersections;
        }

        // Now we need to make sure those slightly negative discriminants don't cause NaNs in the sqrt, which would
        // cause the intersection test to fail completely. We can just clamp them to zero.
        //        discriminant = std::max(discriminant, 0.f);
        float sqrtDiscriminant = SafeSqrt(discriminant);
        float t0 = (-b - sqrtDiscriminant) / (2.f * a);
        float t1 = (-b + sqrtDiscriminant) / (2.f * a);

        if (t0 > t1)
        {
            std::swap(t0, t1);
        }

        float y0 = origin.y + t0 * direction.y;

        if (minimum < y0 && y0 < maximum)
        {
            intersections.push_back(t0);
        }

        float y1 = origin.y + t1 * direction.y;
        if (minimum < y1 && y1 < maximum)
        {
            intersections.push_back(t1);
        }
    }

    return intersections;
}

Tuple Cone::NormalAtLocal(const Tuple &point) const
{
    return Vector(0.f, 0.f, 0.f);
}
