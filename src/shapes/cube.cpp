#include "shapes/cube.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

Tuple Cube::NormalAtLocal(const Tuple &point) const
{
    assert(point.IsValid());

    float maxComponent = std::max({std::abs(point.x), std::abs(point.y), std::abs(point.z)});

    if (maxComponent == std::abs(point.x))
    {
        return Vector(point.x, 0.f, 0.f);
    }
    if (maxComponent == std::abs(point.y))
    {
        return Vector(0.f, point.y, 0.f);
    }
    return Vector(0.f, 0.f, point.z);
}

std::vector<float> Cube::IntersectLocal(const Ray &ray) const
{
    assert(ray.IsValid());

    float xtmin, xtmax, ytmin, ytmax, ztmin, ztmax;
    CheckAxis(ray.GetOrigin().x, ray.GetDirection().x, xtmin, xtmax);
    CheckAxis(ray.GetOrigin().y, ray.GetDirection().y, ytmin, ytmax);
    CheckAxis(ray.GetOrigin().z, ray.GetDirection().z, ztmin, ztmax);

    float tmin = std::max({xtmin, ytmin, ztmin});
    float tmax = std::min({xtmax, ytmax, ztmax});

    std::vector<float> intersections;
    if (tmax > tmin)
    {
        intersections.reserve(2);
        intersections.push_back(tmin);
        intersections.push_back(tmax);
    }
    return intersections;
}

void Cube::CheckAxis(float origin, float direction, float &tmin, float &tmax) const
{
    assert(std::isfinite(origin));
    assert(std::isfinite(direction));

    float tminNumerator = (-1.f - origin);
    float tmaxNumerator = (1.f - origin);
    if (std::abs(direction) >= 1e-6f)
    {
        tmin = tminNumerator / direction;
        tmax = tmaxNumerator / direction;
    }
    else
    {
        tmin = tminNumerator * std::numeric_limits<float>::infinity();
        tmax = tmaxNumerator * std::numeric_limits<float>::infinity();
    }
    if (tmin > tmax)
    {
        std::swap(tmin, tmax);
    }
}
