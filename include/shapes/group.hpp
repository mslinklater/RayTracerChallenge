#pragma once
#include "shape.hpp"
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

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

    Group(const Group& other);
    Group& operator=(const Group& other);

    std::unique_ptr<Shape> Clone() const override
    {
        return std::make_unique<Group>(*this);
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

    template <typename T, typename = std::enable_if_t<std::is_base_of_v<Shape, T>>> T& AddChild(const T& child)
    {
        if (static_cast<const Shape*>(&child) == this)
        {
            throw std::invalid_argument("Group::AddChild() Group '" + this->GetName() +
                                        "' cannot add itself as a child.");
        }
        ShapeUniquePtr ownedChild = child.Clone();
        Shape& addedChild = AddOwnedChild(std::move(ownedChild));
        auto* typedChild = dynamic_cast<T*>(&addedChild);
        assert(typedChild != nullptr);
        return *typedChild;
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

    bool Contains(const Shape& child) const
    {
        return FindChildByName(child.GetName()) != nullptr;
    }

    const Shape& GetChild(size_t index) const;
    Shape& GetMutableChild(size_t index);
    const std::vector<ShapeUniquePtr>& GetChildren() const;

  private:
    Shape& AddOwnedChild(ShapeUniquePtr child);
    const Shape* FindChildByName(const std::string& childName) const;

    std::vector<ShapeUniquePtr> children;
};
