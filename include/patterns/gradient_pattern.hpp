#pragma once
#include "pattern.hpp"

class GradientPattern : public Pattern
{
  public:
    GradientPattern(const Color &_a, const Color &_b) : Pattern(_a, _b)
    {
    }
    Color PatternAt(const Tuple &point)
    {
        Color distance = GetB() - GetA();
        float fraction = point.x - std::floor(point.x);
        return GetA() + distance * fraction;
    }
};
