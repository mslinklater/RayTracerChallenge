#pragma once
#include "color.hpp"
#include "matrix.hpp"
#include "shape.hpp"
#include "tuple.hpp"

class Pattern
{
  public:
    Pattern() : transform(Matrix(4)), a(kColorWhite), b(kColorBlack)
    {
        transform.SetIdentity();
    }
    Pattern(const Color &_a, const Color &_b) : transform(Matrix(4)), a(_a), b(_b)
    {
        transform.SetIdentity();
    }

    void SetTransform(const Matrix &transform)
    {
        this->transform = transform;
    }
    Matrix GetTransform() const
    {
        return transform;
    }

    virtual ~Pattern() = default;
    virtual Color PatternAt(const Tuple &point) = 0;

    Color PatternAtShape(const Shape &shape, const Tuple &point)
    {
        Tuple localPoint = shape.GetTransform().GetInverse() * point;
        Tuple patternPoint = transform.GetInverse() * localPoint;
        return PatternAt(patternPoint);
    }

    Color GetA() const
    {
        return a;
    }
    Color GetB() const
    {
        return b;
    }

  protected:
    Color a;
    Color b;
    Matrix transform;
};

class StripePattern : public Pattern
{
  public:
    StripePattern()
    {
    }
    StripePattern(const Color &_a, const Color &_b) : Pattern(_a, _b)
    {
    }

    Color PatternAt(const Tuple &point) override;
    // Color PatternAtShape(const Shape &object, const Tuple &point) override;
};
