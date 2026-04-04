#pragma once
#include "color.hpp"
#include "matrix.hpp"
#include "tuple.hpp"

class Shape;

class StripePattern
{
  public:
    StripePattern() : a(kColorWhite), b(kColorBlack), transform(Matrix(4))
    {
        transform.SetIdentity();
    }
    StripePattern(const Color &_a, const Color &_b) : a(_a), b(_b), transform(Matrix(4))
    {
        transform.SetIdentity();
    }

    Color GetA() const
    {
        return a;
    }
    Color GetB() const
    {
        return b;
    }
    Color StripeAt(const Tuple &point);
    Color StripeAtObject(const Shape &object, const Tuple &point);
    void SetTransform(const Matrix &transform)
    {
        this->transform = transform;
    }

  private:
    Color a;
    Color b;
    Matrix transform;
};
