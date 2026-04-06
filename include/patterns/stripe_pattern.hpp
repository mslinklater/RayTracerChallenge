#pragma once
#include "pattern.hpp"

class StripePattern : public Pattern
{
  public:
    StripePattern()
    {
    }
    StripePattern(const Color &_a, const Color &_b) : Pattern(_a, _b)
    {
    }

    Color PatternAt(const Tuple &point) override
    {
        if (static_cast<int>(std::floor(point.x)) % 2 == 0)
        {
            return GetA();
        }
        return GetB();
    }
};
