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

bool CSG::IntersectionAllowed(Operation op, bool lhit, bool inl, bool inr)
{
    switch (op)
    {
    case OpUnion:
        {
            return (lhit && !inr) || (!lhit && !inl);
        }
    case OpIntersection:
        {
            return (lhit && inr) || (!lhit && inl);
        }
    case OpDifference:
        {
            return (lhit && !inr) || (!lhit && inl);
        }
    }

    return false;
}

std::vector<Intersection> CSG::FilterIntersections(const std::vector<Intersection>& intersections)
{
    std::vector<Intersection> result;
    bool inl = false;
    bool inr = false;

    for (const auto& i : intersections)
    {
        bool lhit = left->Includes(i.GetObjectId());

        if (IntersectionAllowed(operation, lhit, inl, inr))
        {
            result.push_back(i);
        }

        if (lhit)
        {
            inl = !inl;
        }
        else
        {
            inr = !inr;
        }
    }

    return result;
}

bool CSG::Includes(ObjectId objectId) const
{
    return left->Includes(objectId) || right->Includes(objectId);
}
