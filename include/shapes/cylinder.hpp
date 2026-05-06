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
    Cylinder(const std::string& name) : Shape(name)
    {
    }

    std::unique_ptr<Shape> Clone() const override
    {
        return std::make_unique<Cylinder>(*this);
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
    Tuple NormalAtLocal(const Tuple& point, const Intersection& intersection) const override;

    float GetMinimum() const
    {
        return minimum;
    }
    void SetMinimum(float min)
    {
        assert(std::isfinite(min));
        minimum = min;
    }

    float GetMaximum() const
    {
        return maximum;
    }
    void SetMaximum(float max)
    {
        assert(std::isfinite(max));
        maximum = max;
    }

    bool IsClosed() const
    {
        return closed;
    }
    void SetClosed(bool isClosed)
    {
        closed = isClosed;
    }

  private:
    bool CheckCap(const Ray& ray, float t) const; ///< Helper for checking ray-cap intersection
    void IntersectCaps(const Ray& ray,
                       std::vector<Intersection>& intersections) const; ///< Helper for intersecting end caps

    float minimum = -std::numeric_limits<float>::infinity(); ///< Minimum y value (default: infinite)
    float maximum = std::numeric_limits<float>::infinity();  ///< Maximum y value (default: infinite)
    bool closed = false;                                     ///< Whether the cylinder has end caps (default: open)
};

/** @brief Equality comparison based on transform and material. */
bool operator==(const Cylinder& s1, const Cylinder& s2);
