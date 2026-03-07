#pragma once
#include "tuple.hpp"

struct Ray
{
    Ray(const Tuple &origin, const Tuple &direction) : origin(origin), direction(direction) {}

    Tuple origin;
    Tuple direction;
};

extern Tuple Position(const Ray &ray, float t);