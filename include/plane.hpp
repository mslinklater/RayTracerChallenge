#pragma once
#include "shape.hpp"

class Plane : public Shape
{
public:
    Plane(const std::string &name) : Shape(name)
    {
    }

    std::vector<float> IntersectLocal(const Ray &ray) const override; // intersect a ray which is in local space
    Tuple NormalAtLocal(const Tuple &point) const override;
};

// bool operator==(const Plane &p1, const Plane &p2);