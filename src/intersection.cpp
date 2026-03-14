#include "intersection.hpp"

bool Intersection::operator==(const Intersection &other) const
{
    return AreEqual(t, other.t) && object == other.object;
}

std::vector<Intersection> Intersections(std::initializer_list<Intersection> list)
{
    return std::vector<Intersection>(list);
}

Intersection GetClosestIntersection(const std::vector<Intersection> &intersections)
{
    Intersection hit(0.f, nullptr);
    for (const auto &intersection : intersections)
    {
        if (intersection.GetT() >= 0.f)
        {
            if (hit.GetObject() == nullptr || intersection.GetT() < hit.GetT())
            {
                hit = intersection;
            }
        }
    }
    return hit;
}

Computations PrepareComputations(const Intersection &intersection, const Ray &ray)
{
    Computations comps;
    comps.t = intersection.GetT();
    comps.object = intersection.GetObject();
    comps.point = Position(ray, comps.t);
    comps.eyeVector = -ray.direction;
    comps.normalVector = comps.object->NormalAt(comps.point);

    if ((comps.normalVector | comps.eyeVector) < 0.f)
    {
        comps.inside = true;
        comps.normalVector = -comps.normalVector;
    }

    return comps;
}