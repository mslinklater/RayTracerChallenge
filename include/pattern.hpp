#pragma once
#include "color.hpp"
#include "tuple.hpp"

class StripePattern
{
  public:
    StripePattern(const Color &_a, const Color &_b) : a(_a), b(_b)
    {
    }

    Color GetA() const
    {
        return a;
    }
    Color GetB() const
    {
        return b;
    }
    Color StripeAt(const Tuple &point);

  private:
    Color a;
    Color b;
};
