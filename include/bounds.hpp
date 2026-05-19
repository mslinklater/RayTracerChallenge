#pragma once
#include "tuple.hpp"

class Bounds
{
  public:
    Bounds();
    Bounds(const Tuple& min, const Tuple& max);

    const Tuple& GetMin() const
    {
        return min;
    };
    const Tuple& GetMax() const
    {
        return max;
    };

  private:
    Tuple min;
    Tuple max;
};
