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
