#pragma once
#include "color.hpp"
#include "tuple.hpp"

class Shape;

class StripePattern
{
  public:
    StripePattern() : a(kColorWhite), b(kColorBlack)
    {
    }
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
    Color StripeAtObject(const Shape &object, const Tuple &point);

  private:
    Color a;
    Color b;
};
