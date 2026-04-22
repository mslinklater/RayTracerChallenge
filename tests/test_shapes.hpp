#pragma once
#include "shapes/shape.hpp"

class TestShape : public Shape
{
  public:
    TestShape(const std::string &name) : Shape(name)
    {
    }
};
