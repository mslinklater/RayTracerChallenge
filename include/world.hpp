#pragma once
#include "light.hpp"
#include "shape.hpp"
#include "types.hpp"
#include <deque>
#include <memory>
#include <type_traits>
#include <vector>

class World
{
  public:
    World() = default;

    const std::deque<std::unique_ptr<Shape>> &GetObjects() const;
    const std::vector<Light> &GetLights() const;

    void ReplaceLight(int index, const Light &light);

    void AddLight(const Light &light) // TODO: use proper generated ObjectId's
    {
        lights.push_back(light);
    }

    template <typename T, typename = std::enable_if_t<std::is_base_of_v<Shape, T>>> ObjectId AddObject(const T &object)
    {
        objects.push_back(std::make_unique<T>(object));
        return static_cast<ObjectId>(objects.size() - 1);
    }

    const Shape &GetObject(size_t index) const;
    const Light &GetLight(size_t index) const;

    Shape &GetMutableObject(size_t index);
    Light &GetMutableLight(size_t index);

    bool ContainsLight(const Light &light) const;
    bool ContainsObject(const Shape &object) const;

  private:
    std::deque<std::unique_ptr<Shape>> objects;
    std::vector<Light> lights;

    ObjectId nextObjectId = 0;
};
