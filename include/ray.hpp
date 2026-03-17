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

// extern Tuple Position(const Ray &ray, float t); // TODO: rename this and fold in to class Ray

extern std::vector<float> Intersect(const Sphere &sphere, const Ray &ray);

extern Ray operator*(const Ray &ray, const Matrix &matrix);