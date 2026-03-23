#pragma once
#include "matrix.hpp"
#include "material.hpp"
#include "shape.hpp"

class Sphere : public Shape
{
public:
    Sphere(const std::string &name) : Shape(name)
    {
    }

private:
};

bool operator==(const Sphere &s1, const Sphere &s2);