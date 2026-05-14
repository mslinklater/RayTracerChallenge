#include "shapes/csg.hpp"

CSG::CSG(const std::string& name, Operation op, Shape* leftChild, Shape* rightChild)
    : Shape(name), operation(op), left(leftChild), right(rightChild)
{
    assert(left != nullptr);
    assert(right != nullptr);

    left->SetParent(this);
    right->SetParent(this);
}

std::vector<Intersection> CSG::IntersectLocal(const Ray& ray) const
{
    return {};
}

Tuple CSG::NormalAtLocal(const Tuple& point, const Intersection& intersection) const
{
    return Vector(0.f, 0.f, 0.f);
}
