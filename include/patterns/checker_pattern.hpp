#pragma once
#include "pattern.hpp"

class CheckerPattern : public Pattern
{
  public:
    CheckerPattern()
    {
    }
    CheckerPattern(const Color &_a, const Color &_b) : Pattern(_a, _b)
    {
    }

    Color PatternAt(const Tuple &point) override
    {
        if (static_cast<int>(std::floor(point.x) + std::floor(point.y) + std::floor(point.z)) % 2 == 0)
        {
            return GetA();
        }
        return GetB();
    }
};
