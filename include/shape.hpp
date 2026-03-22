#pragma once
#include <string>

class Shape
{
public:
    Shape(const std::string &name) : name(name) {}
    virtual ~Shape() = default;

protected:
    std::string name;
};
