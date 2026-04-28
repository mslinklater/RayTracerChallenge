#pragma once
#include "tuple.hpp"

class Triangle
{
  public:
    Triangle(const Tuple& p1, const Tuple& p2, const Tuple& p3);

    const Tuple& GetP1() const;
    const Tuple& GetP2() const;
    const Tuple& GetP3() const;

    Tuple GetEdge1() const;
    Tuple GetEdge2() const;
    Tuple GetNormal() const;

  private:
    Tuple p1;
    Tuple p2;
    Tuple p3;
};
