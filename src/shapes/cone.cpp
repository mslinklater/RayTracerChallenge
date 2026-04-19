#include "shapes/cone.hpp"

std::vector<float> Cone::IntersectLocal(const Ray &ray) const
{
    return {};
}

Tuple Cone::NormalAtLocal(const Tuple &point) const
{
    return Vector(0.f, 0.f, 0.f);
}
