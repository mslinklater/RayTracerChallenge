#include "shapes/csg.hpp"
#include <algorithm>
#include <atomic>

namespace
{
std::atomic<ObjectId> nextStandaloneObjectId{kInvalidObjectId - 1};

void EnsureStandaloneObjectId(Shape& shape)
{
    if (shape.GetObjectId() == kInvalidObjectId)
    {
        shape.SetObjectId(nextStandaloneObjectId.fetch_sub(1, std::memory_order_relaxed));
    }
}
} // namespace

CSG::CSG(const std::string& name, Operation op, Shape* leftChild, Shape* rightChild)
    : Shape(name), operation(op), left(leftChild), right(rightChild)
{
    assert(left != nullptr);
    assert(right != nullptr);

    EnsureStandaloneObjectId(*left);
    EnsureStandaloneObjectId(*right);

    left->SetParent(this);
    right->SetParent(this);
}

std::vector<Intersection> CSG::IntersectLocal(const Ray& ray) const
{
    std::vector<Intersection> leftIntersections = left->Intersect(ray);
    std::vector<Intersection> rightIntersections = right->Intersect(ray);
    std::vector<Intersection> allIntersections;
    allIntersections.reserve(leftIntersections.size() + rightIntersections.size());
    allIntersections.insert(allIntersections.end(), leftIntersections.begin(), leftIntersections.end());
    allIntersections.insert(allIntersections.end(), rightIntersections.begin(), rightIntersections.end());
    std::sort(allIntersections.begin(), allIntersections.end(),
              [](const Intersection& a, const Intersection& b) { return a.GetT() < b.GetT(); });
    std::vector<Intersection> result = FilterIntersections(allIntersections);
    return result;
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

std::vector<Intersection> CSG::FilterIntersections(const std::vector<Intersection>& intersections) const
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
    assert(objectId != kInvalidObjectId);
    return left->Includes(objectId) || right->Includes(objectId);
}
