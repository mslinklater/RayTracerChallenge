#pragma once
#include "tuple.hpp"
#include "color.hpp"
#include "material.hpp"

struct Light
{
    Light(const Tuple &position, const Color &intensity) : position(position), intensity(intensity) {}

    Tuple position;
    Color intensity;
};

extern Color Lighting(const Material &material, const Light &light, const Tuple &position, const Tuple &eyeVector, const Tuple &normalVector);
