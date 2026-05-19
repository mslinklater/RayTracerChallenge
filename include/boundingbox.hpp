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

  private:
    Tuple min;
    Tuple max;
};
