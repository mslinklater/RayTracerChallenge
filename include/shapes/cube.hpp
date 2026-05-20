#pragma once
#include "shape.hpp"

/**
 * @brief An axis aligned cube
 *
 * Like @c Sphere, the actual orientation and position are set via the inherited
 * @c transform matrix.
 */
class Cube : public Shape
{
  public:
    /**
     * @brief Constructs a cube
     * @param name Human-readable identifier.
     */
    Cube(const std::string& name) : Shape(name)
    {
    }

    std::unique_ptr<Shape> Clone() const override
    {
        return std::make_unique<Cube>(*this);
    }

    /**
     * @brief ray-cube intersection test in object (local) space.
     * @param ray A ray already transformed into the plane's local space.
     * @return A single ray parameter @c t, or an empty list if the ray is parallel to the plane.
     */
    std::vector<Intersection> IntersectLocal(const Ray& ray) const override;

    /**
     * @brief Returns the object-space surface normal at @p point.
     *
     * @param point The position of the requested normal
     * @return The plane's upward-facing normal vector.
     */
    Tuple NormalAtLocal(const Tuple& point, const Intersection& intersection) const override;

    /**
     * @brief Returns the axis-aligned bounding box of the cube in world space.
     */
    BoundingBox GetBounds() const override;

  private:
    void CheckAxis(float origin, float direction, float& tmin, float& tmax) const;
};
