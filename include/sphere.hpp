#pragma once
#include "matrix.hpp"

struct Sphere
{
    Sphere() : transform(4)
    {
        transform.SetIdentity();
    }

    Matrix transform; // The transformation matrix for the sphere (default is identity)
};