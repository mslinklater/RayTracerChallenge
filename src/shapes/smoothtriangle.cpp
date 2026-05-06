#include "shapes/smoothtriangle.hpp"
#include "shapes/triangle.hpp"

SmoothTriangle::SmoothTriangle(const Tuple& p1, const Tuple& p2, const Tuple& p3, const Tuple& n1, const Tuple& n2,
                               const Tuple& n3)
    : Triangle(p1, p2, p3), n1(n1), n2(n2), n3(n3)
{
}

const Tuple& SmoothTriangle::GetN1() const
{
    return n1;
}

const Tuple& SmoothTriangle::GetN2() const
{
    return n2;
}

const Tuple& SmoothTriangle::GetN3() const
{
    return n3;
}

std::vector<Intersection> SmoothTriangle::IntersectLocal(const Ray& ray) const
{

    std::vector<Intersection> intersections;
    Tuple dirCrossEdge2 = ray.GetDirection() ^ edge2;
    float det = edge1 | dirCrossEdge2;
    if (std::abs(det) > 1e-6)
    {
        float f = 1.f / det;
        Tuple p1ToOrigin = ray.GetOrigin() - p1;
        float u = f * (p1ToOrigin | dirCrossEdge2);

        if (u >= 0.f && u <= 1.f)
        {
            Tuple originCrossEdge1 = p1ToOrigin ^ edge1;
            float v = f * (ray.GetDirection() | originCrossEdge1);

            if (v >= 0.f && (u + v) <= 1.f)
            {
                float t = f * (edge2 | originCrossEdge1);
                intersections.push_back(Intersection(t, objectId, u, v));
            }
        }
    }
    return intersections;
}

Tuple SmoothTriangle::NormalAtLocal(const Tuple& point, const Intersection& intersection) const
{
    return n2 * intersection.GetU() + n3 * intersection.GetV() + n1 * (1.f - intersection.GetU() - intersection.GetV());
}
