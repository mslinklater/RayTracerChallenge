#include "world.hpp"

const std::vector<Sphere> &World::GetObjects()
{
    return objects;
}

const std::vector<Light> &World::GetLights()
{
    return lights;
}

bool World::ContainsLight(const Light &light)
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

bool World::ContainsObject(const Sphere &object)
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