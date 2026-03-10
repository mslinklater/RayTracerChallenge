#pragma once
#include "sphere.hpp"
#include "light.hpp"
#include <vector>

class World
{
public:
    World() = default;

    const std::vector<Sphere> &GetObjects();
    const std::vector<Light> &GetLights();

    void AddLight(const Light &light) { lights.push_back(light); }
    void AddObject(const Sphere &object) { objects.push_back(object); }

    bool ContainsLight(const Light &light);
    bool ContainsObject(const Sphere &object);

private:
    std::vector<Sphere> objects;
    std::vector<Light> lights;
};
