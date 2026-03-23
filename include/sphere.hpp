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

    std::vector<float> Intersect(const Ray &ray) const override;
};

bool operator==(const Sphere &s1, const Sphere &s2);