#pragma once
#include "tuple.hpp"

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

  private:
    Tuple min;
    Tuple max;
};

BoundingBox operator+(const BoundingBox& b1, const BoundingBox& b2);
