#include "pattern.hpp"
#include "shapes/shape.hpp"
#include <cassert>
#include <typeinfo>

Color Pattern::PatternAtShape(const Shape& shape, const Tuple& point) const
{
    assert(point.IsValid());
    assert(transform.GetSize() == 4);
    assert(transform.IsValid());
    assert(inverseTransform.GetSize() == 4);
    assert(inverseTransform.IsValid());

    Tuple localPoint = shape.WorldToObject(point);
    Tuple patternPoint = inverseTransform * localPoint;
    return PatternAt(patternPoint);
}

bool Pattern::Equals(const Pattern& other) const
{
    assert(transform.GetSize() == 4);
    assert(transform.IsValid());
    assert(other.transform.GetSize() == 4);
    assert(other.transform.IsValid());
    return typeid(*this) == typeid(other) && a == other.a && b == other.b && transform == other.transform;
}

bool operator==(const Pattern& lhs, const Pattern& rhs)
{
    return lhs.Equals(rhs);
}

Pattern::Pattern(const Color& _a, const Color& _b) : transform(Matrix(4)), inverseTransform(Matrix(4)), a(_a), b(_b)
{
    transform.SetIdentity();
    inverseTransform.SetIdentity();
}

void Pattern::SetTransform(const Matrix& transform)
{
    this->transform = transform;
    inverseTransform = transform.GetInverse();
}

const Matrix& Pattern::GetTransform() const
{
    return transform;
}

Color Pattern::GetA() const
{
    return a;
}

Color Pattern::GetB() const
{
    return b;
}
