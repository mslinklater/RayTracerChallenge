#include "shapes/triangle.hpp"

int Triangle::triangleCount = 0;

Triangle::Triangle(const Tuple& p1, const Tuple& p2, const Tuple& p3)
    : p1(p1), p2(p2), p3(p3), Shape("triangle" + std::to_string(triangleCount++))
{
    edge1 = p2 - p1;
    edge2 = p3 - p1;
    normal = (edge2 ^ edge1).Normalize();
}

const Tuple& Triangle::GetP1() const
{
    return p1;
}

const Tuple& Triangle::GetP2() const
{
    return p2;
}

const Tuple& Triangle::GetP3() const
{
    return p3;
}

Tuple Triangle::GetEdge1() const
{
    return edge1;
}

Tuple Triangle::GetEdge2() const
{
    return edge2;
}

Tuple Triangle::GetNormal() const
{
    return normal;
}

Tuple Triangle::NormalAtLocal(const Tuple& point) const
{
    return normal;
}

std::vector<Intersection> Triangle::IntersectLocal(const Ray& ray) const
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
                intersections.push_back(Intersection(t, objectId));
            }
        }
    }
    return intersections;
}
