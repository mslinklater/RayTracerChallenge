#pragma once
#include "shape.hpp"

/**
 * @brief An infinite flat plane lying in the XZ plane (y = 0) in object space.
 *
 * Like @c Sphere, the actual orientation and position are set via the inherited
 * @c transform matrix.
 */
class Plane : public Shape
{
  public:
    /**
     * @brief Constructs a plane in the XZ plane with an identity transform.
     * @param name Human-readable identifier.
     */
    Plane(const std::string& name) : Shape(name)
    {
    }

    /**
     * @brief Ray-plane intersection test in object (local) space.
     * @param ray A ray already transformed into the plane's local space.
     * @return A single ray parameter @c t, or an empty list if the ray is parallel to the plane.
     */
    std::vector<Intersection> IntersectLocal(const Ray& ray) const override;

    /**
     * @brief Returns the object-space surface normal at @p point.
     *
     * For a flat plane this is always the constant upward vector (0, 1, 0).
     * @param point Unused — normals are uniform across the plane.
     * @return The plane's upward-facing normal vector.
     */
    Tuple NormalAtLocal(const Tuple& point) const override;
};

// bool operator==(const Plane &p1, const Plane &p2);
