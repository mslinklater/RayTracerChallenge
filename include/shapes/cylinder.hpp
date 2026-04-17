#pragma once
#include "shape.hpp"

/**
 * @brief A unit cylinder centered at the origin in object space.
 *
 * The actual size and position in world space are controlled by the inherited
 * @c transform matrix. Default transform is the identity (radius 1, at origin).
 */
class Cylinder : public Shape
{
  public:
    /**
     * @brief Constructs a unit cylinder with an identity transform.
     * @param name Human-readable identifier.
     */
    Cylinder(const std::string &name) : Shape(name)
    {
    }

    /**
     * @brief Ray-cylinder intersection test in object (local) space.
     * @param ray A ray already transformed into the sphere's local space.
     * @return Up to two ray parameters @c t at each intersection point.
     */
    std::vector<float> IntersectLocal(const Ray &ray) const override;

    /**
     * @brief Returns the object-space surface normal at local-space @p point.
     * @param point A point on the sphere's surface in local space.
     * @return The outward-facing unit normal vector.
     */
    Tuple NormalAtLocal(const Tuple &point) const override;
};

/** @brief Equality comparison based on transform and material. */
bool operator==(const Cylinder &s1, const Cylinder &s2);
