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

    const std::deque<ShapePtr> &GetObjects() const;
    const std::vector<Light> &GetLights() const;

    void ReplaceLight(int index, const Light &light);

    void AddLight(const Light &light) // TODO: use proper generated ObjectId's
    {
        lights.push_back(light);
    }

    // nice how this template means the calling code is cleaner and doesn't have to do a make_unique at the calling site
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<Shape, T>>> ObjectId AddObject(T &object)
    {
        object.SetWorldObjectId(nextObjectId++);
        // objects.push_back(std::make_unique<T>(object));
        ShapePtr shapePtr = std::make_unique<T>(object);
        AddObjectImpl(std::move(shapePtr));
        // objects.push_back(std::move(shapePtr));
        return object.GetWorldObjectId();
    }

    const Shape &GetObject(ObjectId id) const;
    const Shape &GetObjectWithName(const std::string &name) const;
    const Light &GetLight(size_t index) const;

    Shape &GetMutableObject(ObjectId id);
    Shape &GetMutableObjectWithName(const std::string &name) const;
    Light &GetMutableLight(size_t index);

    bool ContainsLight(const Light &light) const;
    bool ContainsObject(const Shape &object) const;

  private:
    void AddObjectImpl(ShapePtr ptr);

    std::deque<ShapePtr> objects;
    std::vector<Light> lights;

    ObjectId nextObjectId = 9;
};
