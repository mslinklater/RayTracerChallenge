#include "pattern.hpp"
#include "shape.hpp"
#include <cmath>

Color StripePattern::PatternAt(const Tuple &point)
{
    if (static_cast<int>(std::floor(point.x)) % 2 == 0)
    {
        return GetA();
    }
    return GetB();
}

Color StripePattern::PatternAtShape(const Shape &shape, const Tuple &point)
{
    Tuple localPoint = shape.GetTransform().GetInverse() * point;
    Tuple patternPoint = transform.GetInverse() * localPoint;
    return PatternAt(patternPoint);
}
