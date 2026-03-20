#include "world.hpp"
#include "ray.hpp"
#include "light.hpp"
#include "intersection.hpp"
#include <algorithm>

const std::deque<Sphere> &World::GetObjects() const
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

const Light &World::GetLight(size_t index) const
{
    if (index >= lights.size())
    {
        throw std::out_of_range("Light index out of range.");
    }
    return lights[index];
}

Light &World::GetMutableLight(size_t index)
{
    if (index >= lights.size())
    {
        throw std::out_of_range("Light index out of range.");
    }
    return lights[index];
}

const Sphere &World::GetObject(size_t index) const
{
    if (index >= objects.size())
    {
        throw std::out_of_range("Object index out of range.");
    }
    return objects[index];
}

Sphere &World::GetMutableObject(size_t index)
{
    if (index >= objects.size())
    {
        throw std::out_of_range("Object index out of range.");
    }
    return objects[index];
}

void World::ReplaceLight(int index, const Light &light)
{
    if (index < 0 || static_cast<size_t>(index) >= lights.size())
    {
        throw std::out_of_range("Light index out of range.");
    }
    lights[index] = light;
}
