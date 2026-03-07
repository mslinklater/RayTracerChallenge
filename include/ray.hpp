#pragma once
#include "tuple.hpp"

struct Ray
{
    Ray(const Tuple &origin, const Tuple &direction) : origin(origin), direction(direction) {}

    Tuple origin;
    Tuple direction;
};
