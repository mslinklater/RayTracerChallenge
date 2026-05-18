#include "world.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "shapes/csg.hpp"
#include "shapes/group.hpp"
#include <cassert>
#include <memory>
#include <unordered_set>

namespace
{
// Used to recursively collect all shapes in a hierarchy for name and ID uniqueness checks when adding a new object to
// the world
void CollectShapeHierarchy(Shape& shape, std::vector<Shape*>& hierarchy)
{
    hierarchy.push_back(&shape);

    if (auto* group = dynamic_cast<Group*>(&shape))
    {
        for (const ShapeUniquePtr& child : group->GetChildren())
        {
            CollectShapeHierarchy(*child, hierarchy);
        }
    }
    else if (auto* csg = dynamic_cast<CSG*>(&shape))
    {
        assert(csg->GetLeft() != nullptr);
        assert(csg->GetRight() != nullptr);
        CollectShapeHierarchy(*csg->GetLeft(), hierarchy);
        CollectShapeHierarchy(*csg->GetRight(), hierarchy);
    }
}
} // namespace

const std::deque<ShapeUniquePtr>& World::GetObjects() const
{
    return objects;
}

const std::vector<Light>& World::GetLights() const
{
    return lights;
}

void World::AddObjectImpl(ShapeUniquePtr ptr)
{
    assert(ptr != nullptr);
    assert(!ptr->GetName().empty());

    std::vector<Shape*> hierarchy;
    CollectShapeHierarchy(*ptr, hierarchy);

    std::unordered_set<std::string> newNames;
    for (Shape* shape : hierarchy)
    {
        if (!newNames.insert(shape->GetName()).second || objectsByName.contains(shape->GetName()))
        {
            throw std::invalid_argument("World::AddObjectImpl() Object with name '" + shape->GetName() +
                                        "' already exists in the world.");
        }
    }

    objects.push_back(std::move(ptr));

    for (Shape* shape : hierarchy)
    {
        objectsById.emplace(shape->GetObjectId(), shape);
        objectsByName.emplace(shape->GetName(), shape);
    }
}

void World::AssignObjectIds(Shape& shape)
{
    shape.SetObjectId(nextObjectId++);

    // If this is a group, descend into children and assign IDs as well.
    if (auto* group = dynamic_cast<Group*>(&shape))
    {
        for (const ShapeUniquePtr& child : group->GetChildren())
        {
            AssignObjectIds(*child);
        }
    }
    else if (auto* csg = dynamic_cast<CSG*>(&shape))
    {
        assert(csg->GetLeft() != nullptr);
        assert(csg->GetRight() != nullptr);
        AssignObjectIds(*csg->GetLeft());
        AssignObjectIds(*csg->GetRight());
    }
}

bool World::ContainsLight(const Light& light) const
{
    assert(light.IsValid());
    for (auto l : lights)
    {
        if (l == light)
        {
            return true;
        }
    }
    return false;
}

bool World::ContainsObject(const Shape& object) const
{
    assert(!object.GetName().empty());
    for (const auto& [id, shape] : objectsById)
    {
        (void)id;
        if (shape->GetTransform() == object.GetTransform() && shape->GetMaterial() == object.GetMaterial())
        {
            return true;
        }
    }
    return false;
}

const Light& World::GetLight(size_t index) const
{
    if (index >= lights.size())
    {
        throw std::out_of_range("Light index out of range.");
    }
    return lights[index];
}

Light& World::GetMutableLight(size_t index)
{
    if (index >= lights.size())
    {
        throw std::out_of_range("Light index out of range.");
    }
    return lights[index];
}

const Shape& World::GetObject(ObjectId id) const
{
    if (id == kInvalidObjectId)
    {
        throw std::invalid_argument("World::GetObject() Invalid Object ID.");
    }

    auto it = objectsById.find(id);
    if (it != objectsById.end())
    {
        return *it->second;
    }
    throw std::out_of_range("World::GetObject() Object ID not found.");
}

const Shape& World::GetObjectWithName(const std::string& name) const
{
    assert(!name.empty());
    auto it = objectsByName.find(name);
    if (it != objectsByName.end())
    {
        return *it->second;
    }
    throw std::out_of_range("World::GetObjectWithName() Object name not found.");
}

Shape& World::GetMutableObject(ObjectId id)
{
    auto it = objectsById.find(id);
    if (it != objectsById.end())
    {
        return *it->second;
    }
    throw std::out_of_range("World::GetMutableObject() Object ID not found.");
}

Shape& World::GetMutableObjectWithName(const std::string& name) const
{
    assert(!name.empty());
    auto it = objectsByName.find(name);
    if (it != objectsByName.end())
    {
        return *it->second;
    }
    throw std::out_of_range("World::GetMutableObjectWithName() Object name not found.");
}

void World::ReplaceLight(int index, const Light& light)
{
    assert(light.IsValid());
    if (index < 0 || static_cast<size_t>(index) >= lights.size())
    {
        throw std::out_of_range("Light index out of range.");
    }
    lights[index] = light;
}

void World::Serialize(Json::Value& json) const
{
    json["numObjects"] = (int)objects.size();
    json["numLights"] = (int)lights.size();
}

void World::Deserialize(const Json::Value& json)
{
}
