#pragma once
#include "tuple.hpp"
#include "sphere.hpp"

struct Matrix;

struct Ray
{
    Ray(const Tuple &origin, const Tuple &direction) : origin(origin), direction(direction) {}

    Tuple origin;
    Tuple direction;
};

extern Tuple Position(const Ray &ray, float t);
extern std::vector<float> Intersect(const Sphere &sphere, const Ray &ray);

extern Ray operator*(const Ray &ray, const Matrix &matrix);