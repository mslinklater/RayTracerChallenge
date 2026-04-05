#include "pattern.hpp"
#include "shape.hpp"
#include <cmath>

Color StripePattern::ColorAt(const Tuple &point)
{
    if (static_cast<int>(std::floor(point.x)) % 2 == 0)
    {
        return GetA();
    }
    return GetB();
}

Color StripePattern::ColorAtObject(const Shape &object, const Tuple &point)
{
    Tuple localPoint = object.GetTransform().GetInverse() * point;
    Tuple patternPoint = transform.GetInverse() * localPoint;
    return ColorAt(patternPoint);
}
