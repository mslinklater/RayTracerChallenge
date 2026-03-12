#include "world.hpp"
#include "ray.hpp"
#include <algorithm>

const std::vector<Sphere> &World::GetObjects() const
{
    return objects;
}

const std::vector<Light> &World::GetLights() const
{
    return lights;
}

bool World::ContainsLight(const Light &light) const
{
    for (auto l : lights)
    {
        if (l == light)
        {
            return true;
        }
    }
    return false;
}

bool World::ContainsObject(const Sphere &object) const
{
    for (auto o : objects)
    {
        if (o.GetTransform() == object.GetTransform() && o.GetMaterial() == object.GetMaterial())
        {
            return true;
        }
    }
    return false;
}

std::vector<Intersection> IntersectWorld(const World &world, const Ray &ray)
{
    std::vector<Intersection> intersections;
    for (const auto &object : world.GetObjects())
    {
        std::vector<float> objectIntersections = Intersect(object, ray);
        for (const auto &distance : objectIntersections)
        {
            Intersection intersection(distance, &object);
            intersections.push_back(intersection);
        }
    }
    // sort intersections by t value
    std::sort(intersections.begin(), intersections.end(), [](const Intersection &a, const Intersection &b)
              { return a.GetT() < b.GetT(); });
    return intersections;
}
