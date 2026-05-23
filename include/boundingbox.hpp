#pragma once
#include "ray.hpp"
#include "tuple.hpp"

class Matrix;

class BoundingBox
{
  public:
    BoundingBox();
    BoundingBox(const Tuple& min, const Tuple& max);

    const Tuple& GetMin() const
    {
        return min;
    };
    const Tuple& GetMax() const
    {
        return max;
    };
    void AddPoint(const Tuple& point);

    /**
     * @brief returns whether the bounding box contains the given point. Points on the boundary are considered to be
     * contained.
     */
    bool Contains(const Tuple& point) const;

    /**
     * @brief returns whether the bounding box contains a given bounding box. Boxes that touch at the boundary are
     * considered to be contained.
     */
    bool Contains(const BoundingBox& box) const;

    /**
     * @brief Transforms the bounding box by the matrix and returns a new bounding box
     */
    BoundingBox Transform(const Matrix& matrix) const;

    /**
     * @brief Extends the existing bounds so it includes the bound object passed in
     */
    void Extend(const BoundingBox& other)
    {
        min.x = std::min(min.x, other.GetMin().x);
        min.y = std::min(min.y, other.GetMin().y);
        min.z = std::min(min.z, other.GetMin().z);

        max.x = std::max(max.x, other.GetMax().x);
        max.y = std::max(max.y, other.GetMax().y);
        max.z = std::max(max.z, other.GetMax().z);
    }

    /**
     * @brief Returns whether the ray intersects the bounding box.
     */
    bool Intersect(const Ray& ray) const;

  private:
    Tuple min;
    Tuple max;
};

BoundingBox operator+(const BoundingBox& b1, const BoundingBox& b2);
