#pragma once
#include "shapes/shape.hpp"

class TestShape : public Shape
{
  public:
    TestShape(const std::string &name) : Shape(name)
    {
    }

    std::unique_ptr<Shape> Clone() const override
    {
        return std::make_unique<TestShape>(*this);
    }
};
