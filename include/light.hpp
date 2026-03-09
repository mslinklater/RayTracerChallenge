#pragma once
#include "tuple.hpp"
#include "color.hpp"

struct Light
{
    Light(const Tuple &position, const Color &intensity) : position(position), intensity(intensity) {}

    Tuple position;
    Color intensity;
};
