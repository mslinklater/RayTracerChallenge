#include "triangle.hpp"

Triangle::Triangle(const Tuple& p1, const Tuple& p2, const Tuple& p3)
    : p1(p1), p2(p2), p3(p3) // e1(p2 - p1), e2(p3 - p1), normal((e2.Cross(e1)).Normalize())
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
