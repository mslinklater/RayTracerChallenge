#include "shapes/group.hpp"
#include <algorithm>
#include <stdexcept>

Group::Group(const Group& other) : Shape(other)
{
    for (const ShapeUniquePtr& child : other.children)
    {
        AddOwnedChild(child->Clone());
    }
}

Group& Group::operator=(const Group& other)
{
    if (this != &other)
    {
        Shape::operator=(other);
        children.clear();
        for (const ShapeUniquePtr& child : other.children)
        {
            AddOwnedChild(child->Clone());
        }
    }
    return *this;
}

std::vector<Intersection> Group::IntersectLocal(const Ray& ray) const
{
    std::vector<Intersection> intersections;
    // agregate intersections from all children
    for (const ShapeUniquePtr& child : children)
    {
        std::vector<Intersection> childIntersections = child->Intersect(ray);
        intersections.insert(intersections.end(), childIntersections.begin(), childIntersections.end());
    }
    // sort intersections by t value
    std::sort(intersections.begin(), intersections.end(),
              [](const Intersection& a, const Intersection& b) { return a.GetT() < b.GetT(); });
    return intersections;
}

Tuple Group::NormalAtLocal(const Tuple& point, const Intersection& intersection) const
{
    return Vector(0.f, 0.f, 0.f);
}

const Shape& Group::GetChild(size_t index) const
{
    if (index >= children.size())
    {
        throw std::out_of_range("Group::GetChild() Child index out of range.");
    }
    return *children[index];
}

Shape& Group::GetMutableChild(size_t index)
{
    if (index >= children.size())
    {
        throw std::out_of_range("Group::GetMutableChild() Child index out of range.");
    }
    return *children[index];
}

const std::vector<ShapeUniquePtr>& Group::GetChildren() const
{
    return children;
}

Shape& Group::AddOwnedChild(ShapeUniquePtr child)
{
    assert(child != nullptr);
    if (FindChildByName(child->GetName()) != nullptr)
    {
        throw std::invalid_argument("Group::AddChild() Child '" + child->GetName() +
                                    "' is already a member of this group.");
    }

    child->SetParent(this);
    children.push_back(std::move(child));
    return *children.back();
}

const Shape* Group::FindChildByName(const std::string& childName) const
{
    for (const ShapeUniquePtr& child : children)
    {
        if (child->GetName() == childName)
        {
            return child.get();
        }
    }
    return nullptr;
}
