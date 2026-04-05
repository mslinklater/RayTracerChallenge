#pragma once
#include "color.hpp"
#include "matrix.hpp"
#include "tuple.hpp"

class Shape;

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

    virtual ~Pattern() = default;
    virtual Color ColorAt(const Tuple &point) = 0;
    virtual Color ColorAtObject(const Shape &object, const Tuple &point) = 0;

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

    Color ColorAt(const Tuple &point) override;
    Color ColorAtObject(const Shape &object, const Tuple &point) override;
};
