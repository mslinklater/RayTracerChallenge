#pragma once
#include "tuple.hpp"
#include "types.hpp"

struct Computations
{
    float t = 0.f;
    ObjectId objectId = kInvalidObjectId;
    Tuple point = {0.f, 0.f, 0.f, 1.f};
    Tuple overPoint = {0.f, 0.f, 0.f, 1.f};
    Tuple eyeVector = {0.f, 0.f, 0.f, 0.f};
    Tuple normalVector = {0.f, 0.f, 0.f, 0.f};
    Tuple reflectv = {0.f, 0.f, 0.f, 0.f};
    bool inside = false; // Indicates whether the intersection occurs on the inside of the object
};
