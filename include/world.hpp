#pragma once
#include "sphere.hpp"
#include "light.hpp"
#include "intersection.hpp"
#include "ray.hpp"
#include <vector>

class World
{
public:
    World() = default;

    const std::vector<Sphere> &GetObjects() const;
    const std::vector<Light> &GetLights() const;

    void AddLight(const Light &light) { lights.push_back(light); }
    void AddObject(const Sphere &object) { objects.push_back(object); }

    bool ContainsLight(const Light &light) const;
    bool ContainsObject(const Sphere &object) const;

private:
    std::vector<Sphere> objects;
    std::vector<Light> lights;
};

extern std::vector<Intersection> IntersectWorld(const World &world, const Ray &ray);