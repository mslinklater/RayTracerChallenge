#include "pattern.hpp"
#include <cmath>

Color StripePattern::StripeAt(const Tuple &point)
{
    if (static_cast<int>(std::floor(point.x)) % 2 == 0)
    {
        return GetA();
    }
    return GetB();
}

Color StripePattern::StripeAtObject(const Shape &object, const Tuple &point)
{
    return kColorWhite;
}
