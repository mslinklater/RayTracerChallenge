#pragma once
#include "boundingbox.hpp"
#include "shape.hpp"

/**
 * @brief A unit sphere centered at the origin in object space.
 *
 * The actual size and position in world space are controlled by the inherited
 * @c transform matrix. Default transform is the identity (radius 1, at origin).
 */
class Sphere : public Shape
{
  public:
    /**
     * @brief Constructs a unit sphere with an identity transform.
     * @param name Human-readable identifier.
     */
    Sphere(const std::string& name) : Shape(name)
    {
    }

    std::unique_ptr<Shape> Clone() const override
    {
        return std::make_unique<Sphere>(*this);
    }

    /**
     * @brief Ray-sphere intersection test in object (local) space.
     * @param ray A ray already transformed into the sphere's local space.
     * @return Up to two ray parameters @c t at each intersection point.
     */
    std::vector<Intersection> IntersectLocal(const Ray& ray) const override;

    /**
     * @brief Returns the object-space surface normal at local-space @p point.
     * @param point A point on the sphere's surface in local space.
     * @return The outward-facing unit normal vector.
     */
    Tuple NormalAtLocal(const Tuple& point, const Intersection& intersection) const override;

    /**
     * @brief Returns the axis-aligned bounding box of the sphere in world space.
     */
    BoundingBox GetBounds() const override;
};

/** @brief Equality comparison based on transform and material. */
bool operator==(const Sphere& s1, const Sphere& s2);
