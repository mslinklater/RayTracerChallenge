#pragma once
#include "intersection.hpp"
#include "shape.hpp"

/**
 * @brief A CSG pair
 *
 * Like @c Sphere, the actual orientation and position are set via the inherited
 * @c transform matrix.
 */
class CSG : public Shape
{
  public:
    enum Operation
    {
        OpUnion,
        OpIntersection,
        OpDifference
    };

    /**
     * @brief Constructs a CSG
     * @param name Human-readable identifier.
     */
    CSG(const std::string& name, Operation op, Shape* leftChild, Shape* rightChild);

    std::unique_ptr<Shape> Clone() const override
    {
        return std::make_unique<CSG>(*this);
    }

    /**
     * @brief ray-csg intersection test in object (local) space.
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

    Operation GetOperation() const
    {
        return operation;
    }

    Shape* GetLeft() const
    {
        return left;
    }

    Shape* GetRight() const
    {
        return right;
    }

    static bool IntersectionAllowed(Operation op, bool lhit, bool inl, bool inr);

  private:
    Operation operation;
    Shape* left = nullptr;
    Shape* right = nullptr;
};
