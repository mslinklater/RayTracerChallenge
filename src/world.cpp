#include "world.hpp"
#include "light.hpp"
#include "ray.hpp"
#include <memory>

const std::deque<ShapePtr> &World::GetObjects() const
{
    return objects;
}

const std::vector<Light> &World::GetLights() const
{
    return lights;
}

void World::AddObjectImpl(ShapePtr ptr)
{
    // if any existing objects share the same name throw an expeption
    for (const auto &o : objects)
    {
        if (o->GetName() == ptr->GetName())
        {
            throw std::invalid_argument("World::AddObjectImpl() Object with name '" + ptr->GetName() +
                                        "' already exists in the world.");
        }
    }
    objects.push_back(std::move(ptr));
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

bool World::ContainsObject(const Shape &object) const
{
    for (const auto &o : objects)
    {
        if (o->GetTransform() == object.GetTransform() && o->GetMaterial() == object.GetMaterial())
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

const Shape &World::GetObject(ObjectId id) const
{
    if (id == kInvalidObjectId)
    {
        throw std::invalid_argument("World::GetObject() Invalid Object ID.");
    }

    for (const auto &object : objects)
    {
        if (object->GetWorldObjectId() == id)
        {
            return *object;
        }
    }
    throw std::out_of_range("World::GetObject() Object ID not found.");
}

const Shape &World::GetObjectWithName(const std::string &name) const
{
    for (const auto &object : objects)
    {
        if (object->GetName() == name)
        {
            return *object;
        }
    }
    throw std::out_of_range("World::GetObjectWithName() Object name not found.");
}

Shape &World::GetMutableObject(ObjectId id)
{
    for (const auto &object : objects)
    {
        if (object->GetWorldObjectId() == id)
        {
            return *object;
        }
    }
    throw std::out_of_range("World::GetMutableObject() Object ID not found.");
}

Shape &World::GetMutableObjectWithName(const std::string &name) const
{
    for (const auto &object : objects)
    {
        if (object->GetName() == name)
        {
            return *object;
        }
    }
    throw std::out_of_range("World::GetMutableObjectWithName() Object name not found.");
}

void World::ReplaceLight(int index, const Light &light)
{
    if (index < 0 || static_cast<size_t>(index) >= lights.size())
    {
        throw std::out_of_range("Light index out of range.");
    }
    lights[index] = light;
}
