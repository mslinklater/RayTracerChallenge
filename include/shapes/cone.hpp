#pragma once
#include "shape.hpp"

/**
 * @brief An axis aligned cone
 *
 * Like @c Sphere, the actual orientation and position are set via the inherited
 * @c transform matrix.
 */
class Cone : public Shape
{
  public:
    /**
     * @brief Constructs a cone
     * @param name Human-readable identifier.
     */
    Cone(const std::string &name) : Shape(name)
    {
    }

    /**
     * @brief ray-cube intersection test in object (local) space.
     * @param ray A ray already transformed into the plane's local space.
     * @return A single ray parameter @c t, or an empty list if the ray is parallel to the plane.
     */
    std::vector<float> IntersectLocal(const Ray &ray) const override;

    /**
     * @brief Returns the object-space surface normal at @p point.
     *
     * @param point The position of the requested normal
     * @return The plane's upward-facing normal vector.
     */
    Tuple NormalAtLocal(const Tuple &point) const override;

    float GetMinimum() const
    {
        return minimum;
    }
    void SetMinimum(float min)
    {
        minimum = min;
    }
    float GetMaximum() const
    {
        return maximum;
    }
    void SetMaximum(float max)
    {
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
    bool CheckCap(const Ray &ray, float t, float radius) const; ///< Helper for checking ray-cap intersection
    void IntersectCaps(const Ray &ray, std::vector<float> &intersections) const; ///< Helper for intersecting end caps
    ///
    float minimum = -std::numeric_limits<float>::infinity();
    float maximum = std::numeric_limits<float>::infinity();
    bool closed = false;
};
