#pragma once
#include "shape.hpp"
#include <set>

/**
 * @brief A unit cylinder centered at the origin in object space.
 *
 * The actual size and position in world space are controlled by the inherited
 * @c transform matrix. Default transform is the identity (radius 1, at origin).
 */
class Group : public Shape
{
  public:
    /**
     * @brief Constructs a unit cylinder with an identity transform.
     * @param name Human-readable identifier.
     */
    Group(const std::string& name) : Shape(name)
    {
    }

    /**
     * @brief Ray-cylinder intersection test in object (local) space.
     * @param ray A ray already transformed into the sphere's local space.
     * @return Up to two ray parameters @c t at each intersection point.
     */
    std::vector<Intersection> IntersectLocal(const Ray& ray) const override;

    /**
     * @brief Returns the object-space surface normal at local-space @p point.
     * @param point A point on the sphere's surface in local space.
     * @return The outward-facing unit normal vector.
     */
    Tuple NormalAtLocal(const Tuple& point) const override;

    size_t GetNumChildren() const
    {
        return children.size();
    }

    void AddChild(Shape* child)
    {
        assert(child != nullptr);
        if (children.find(child) != children.end())
        {
            throw std::invalid_argument("Group::AddChild() Child '" + child->GetName() +
                                        "' is already a member of this group.");
        }
        children.insert(child);
        child->SetParent(this);
    }

#if 0
    void RemoveChild(Shape* child)
    {
        assert(child != nullptr);
        if (children.find(child) == children.end())
        {
            throw std::invalid_argument("Group::RemoveChild() Child '" + child->GetName() +
                                        "' is not a member of this group.");
        }
        children.erase(child);
        child->SetParent(nullptr);
    }
#endif

    bool Contains(Shape* child) const
    {
        assert(child != nullptr);
        return children.find(child) != children.end();
    }

  private:
    std::set<Shape*> children;
};
