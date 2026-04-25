#include "shapes/cylinder.hpp"
#include "maths.hpp"
#include "ray.hpp"
#include <cassert>
#include <cmath>

bool operator==(const Cylinder& s1, const Cylinder& s2)
{
    return s1.GetTransform() == s2.GetTransform() && s1.GetMaterial() == s2.GetMaterial() &&
           s1.GetName() == s2.GetName();
}

std::vector<Intersection> Cylinder::IntersectLocal(const Ray& ray) const
{
    assert(ray.IsValid());

    std::vector<Intersection> intersections;

    const float a = ray.GetDirection().x * ray.GetDirection().x + ray.GetDirection().z * ray.GetDirection().z;

    if (std::abs(a) > 1e-6f)
    {
        const float b =
            (2.f * ray.GetOrigin().x * ray.GetDirection().x) + (2.f * ray.GetOrigin().z * ray.GetDirection().z);
        const float c = ray.GetOrigin().x * ray.GetOrigin().x + ray.GetOrigin().z * ray.GetOrigin().z - 1.f;
        const float discriminant = b * b - 4.f * a * c;

        if (discriminant >= 0.f)
        {
            const float sqrtDiscriminant = std::sqrt(discriminant);
            const float t0 = (-b - sqrtDiscriminant) / (2.f * a);
            const float t1 = (-b + sqrtDiscriminant) / (2.f * a);

            float y0 = ray.GetOrigin().y + t0 * ray.GetDirection().y;
            if (minimum < y0 && y0 < maximum)
            {
                intersections.push_back(Intersection(t0, objectId));
            }

            float y1 = ray.GetOrigin().y + t1 * ray.GetDirection().y;
            if (minimum < y1 && y1 < maximum)
            {
                intersections.push_back(Intersection(t1, objectId));
            }
        }
    }

    if (closed)
    {
        IntersectCaps(ray, intersections);
    }
    return intersections;
}

Tuple Cylinder::NormalAtLocal(const Tuple& point) const
{
    assert(point.IsValid());

    float dist = point.x * point.x + point.z * point.z;
    if (dist < 1.f && point.y >= maximum - 1e-6f)
    {
        return Vector(0.f, 1.f, 0.f);
    }
    else if (dist < 1.f && point.y <= minimum + 1e-6f)
    {
        return Vector(0.f, -1.f, 0.f);
    }
    else
    {
        return Vector(point.x, 0.f, point.z);
    }
}

bool Cylinder::CheckCap(const Ray& ray, float t) const
{
    assert(ray.IsValid());
    assert(std::isfinite(t));

    float x = ray.GetOrigin().x + t * ray.GetDirection().x;
    float z = ray.GetOrigin().z + t * ray.GetDirection().z;
    const float distanceSquared = x * x + z * z;
    return distanceSquared < 1.f || AreEqual(distanceSquared, 1.f);
}

void Cylinder::IntersectCaps(const Ray& ray, std::vector<Intersection>& intersections) const
{
    assert(ray.IsValid());

    if (std::abs(ray.GetDirection().y) < 1e-6f)
    {
        // Ray is parallel to the caps, so it cannot intersect them
        return;
    }

    float t = (minimum - ray.GetOrigin().y) / ray.GetDirection().y;
    if (CheckCap(ray, t))
    {
        intersections.push_back(Intersection(t, objectId));
    }

    t = (maximum - ray.GetOrigin().y) / ray.GetDirection().y;
    if (CheckCap(ray, t))
    {
        intersections.push_back(Intersection(t, objectId));
    }
}
