#pragma once
#include "sphere.hpp"
#include "light.hpp"
#include "types.hpp"
#include <vector>
#include <deque>

class World
{
public:
    World() = default;

    const std::deque<Sphere> &GetObjects() const;
    const std::vector<Light> &GetLights() const;

    void ReplaceLight(int index, const Light &light);

    void AddLight(const Light &light) { lights.push_back(light); }
    ObjectId AddObject(const Sphere &object)
    {
        objects.push_back(object);
        return objects.size() - 1;
    }

    const Sphere &GetObject(size_t index) const;
    const Light &GetLight(size_t index) const;

    Sphere &GetMutableObject(size_t index);
    Light &GetMutableLight(size_t index);

    bool ContainsLight(const Light &light) const;
    bool ContainsObject(const Sphere &object) const;

private:
    std::deque<Sphere> objects;
    std::vector<Light> lights;
};
