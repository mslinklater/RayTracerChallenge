#include "world.hpp"
#include "ray.hpp"
#include "light.hpp"
#include "intersection.hpp"
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

Color ShadeHit(const World &world, const Computations &comps)
{
    return Lighting(comps.object->GetMaterial(), world.GetLight(0), comps.point, comps.eyeVector, comps.normalVector);
}

Color ColorAt(const World &world, const Ray &ray)
{
    std::vector<Intersection> intersections = IntersectWorld(world, ray);
    if (intersections.empty())
    {
        return Color(0.f, 0.f, 0.f); // Return black if no intersections
    }
    // find the first intersection with a positive t value
    auto it = std::find_if(intersections.begin(), intersections.end(), [](const Intersection &intersection)
                           { return intersection.GetT() >= 0.f; });
    if (it == intersections.end())
    {
        return Color(0.f, 0.f, 0.f); // Return black if all intersections are behind the ray
    }
    const Intersection &hit = *it;
    Computations comps = PrepareComputations(hit, ray);
    return ShadeHit(world, comps);
}

World DefaultWorld()
{
    World w;
    Light light(Point(-10.f, 10.f, -10.f), Color(1.f, 1.f, 1.f));
    w.AddLight(light);

    Sphere s1;
    s1.GetMutableMaterial().SetColor(Color(0.8f, 1.f, 0.6f));
    s1.GetMutableMaterial().SetDiffuse(0.7f);
    s1.GetMutableMaterial().SetSpecular(0.2f);
    w.AddObject(s1);

    Sphere s2;
    s2.SetTransform(Matrix::CreateScaling(0.5f, 0.5f, 0.5f));
    w.AddObject(s2);
    return w;
}