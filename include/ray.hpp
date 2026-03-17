#pragma once
#include "tuple.hpp"
#include "sphere.hpp"

struct Matrix;

class Ray
{
public:
    Ray(const Tuple &origin, const Tuple &direction) : origin(origin), direction(direction) {}

    Tuple GetOrigin() const { return origin; }
    Tuple GetDirection() const { return direction; }
    Tuple PositionAt(float t) const { return origin + direction * t; }

private:
    Tuple origin;
    Tuple direction;
};

extern Ray operator*(const Ray &ray, const Matrix &matrix);