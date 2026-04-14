#include "pattern.hpp"
#include "shapes/shape.hpp"
#include <typeinfo>

Color Pattern::PatternAtShape(const Shape &shape, const Tuple &point) const
{
    Tuple localPoint = shape.WorldToObject(point);
    Tuple patternPoint = inverseTransform * localPoint;
    return PatternAt(patternPoint);
}

bool Pattern::Equals(const Pattern &other) const
{
    return typeid(*this) == typeid(other) && a == other.a && b == other.b && transform == other.transform;
}

bool operator==(const Pattern &lhs, const Pattern &rhs)
{
    return lhs.Equals(rhs);
}
