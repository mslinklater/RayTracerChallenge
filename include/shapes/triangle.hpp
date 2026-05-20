#pragma once
#include "shapes/shape.hpp"
#include "tuple.hpp"

class Triangle : public Shape
{
  public:
    Triangle(const Tuple& p1, const Tuple& p2, const Tuple& p3);

    const Tuple& GetP1() const;
    const Tuple& GetP2() const;
    const Tuple& GetP3() const;

    Tuple GetEdge1() const;
    Tuple GetEdge2() const;
    Tuple GetNormal() const;

    std::unique_ptr<Shape> Clone() const override
    {
        return std::make_unique<Triangle>(*this);
    }

    Tuple NormalAtLocal(const Tuple& point, const Intersection& intersection) const override;

    std::vector<Intersection> IntersectLocal(const Ray& ray) const override;

    BoundingBox GetBounds() const override;

  protected:
    Tuple p1;
    Tuple p2;
    Tuple p3;

    Tuple edge1;
    Tuple edge2;
    Tuple normal;

    static int triangleCount;
};
