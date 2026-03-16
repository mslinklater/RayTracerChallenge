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

    void ReplaceLight(int index, const Light &light);

    void AddLight(const Light &light) { lights.push_back(light); }
    void AddObject(const Sphere &object) { objects.push_back(object); }

    const Sphere &GetObject(size_t index) const;
    const Light &GetLight(size_t index) const;

    Sphere &GetMutableObject(size_t index);
    Light &GetMutableLight(size_t index);

    bool ContainsLight(const Light &light) const;
    bool ContainsObject(const Sphere &object) const;

private:
    std::vector<Sphere> objects;
    std::vector<Light> lights;
};

extern std::vector<Intersection> IntersectWorld(const World &world, const Ray &ray);
extern Color ShadeHit(const World &world, const Computations &comps);
extern Color ColorAt(const World &world, const Ray &ray);