#pragma once
#include "pattern.hpp"

class RingPattern : public Pattern
{
  public:
    RingPattern()
    {
    }
    RingPattern(const Color &_a, const Color &_b) : Pattern(_a, _b)
    {
    }

    Color PatternAt(const Tuple &point) override
    {
        if (static_cast<int>(std::floor(sqrt(point.x * point.x + point.z * point.z))) % 2 == 0)
        {
            return GetA();
        }
        return GetB();
    }
};
