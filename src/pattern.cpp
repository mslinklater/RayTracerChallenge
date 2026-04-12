#include "pattern.hpp"
#include "shapes/shape.hpp"

Color Pattern::PatternAtShape(const Shape &shape, const Tuple &point)
{
    Tuple localPoint = shape.GetTransform().GetInverse() * point;
    Tuple patternPoint = transform.GetInverse() * localPoint;
    return PatternAt(patternPoint);
}
