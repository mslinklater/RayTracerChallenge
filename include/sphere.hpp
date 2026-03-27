#pragma once
#include "shape.hpp"

class Sphere : public Shape
{
public:
    Sphere(const std::string &name) : Shape(name)
    {
    }

    std::vector<float> IntersectLocal(const Ray &ray) const override; // intersect a ray which is in local space
};

bool operator==(const Sphere &s1, const Sphere &s2);
