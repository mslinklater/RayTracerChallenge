#pragma once
#include "shapes/shape.hpp"
#include "shapes/triangle.hpp"
#include "tuple.hpp"

class SmoothTriangle : public Triangle
{
  public:
    SmoothTriangle(const Tuple& p1, const Tuple& p2, const Tuple& p3, const Tuple& n1, const Tuple& n2,
                   const Tuple& n3);

    const Tuple& GetN1() const;
    const Tuple& GetN2() const;
    const Tuple& GetN3() const;

    std::unique_ptr<Shape> Clone() const override
    {
        return std::make_unique<SmoothTriangle>(*this);
    }

    std::vector<Intersection> IntersectLocal(const Ray& ray) const override;

  private:
    Tuple n1;
    Tuple n2;
    Tuple n3;
};
