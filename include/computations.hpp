#pragma once
#include "tuple.hpp"
class Sphere;

struct Computations
{
    float t = 0.f;
    const Sphere *object = nullptr;
    Tuple point = {0.f, 0.f, 0.f, 1.f};
    Tuple eyeVector = {0.f, 0.f, 0.f, 0.f};
    Tuple normalVector = {0.f, 0.f, 0.f, 0.f};
    bool inside = false; // Indicates whether the intersection occurs on the inside of the object
};
