#pragma once
#include <string>
#include "matrix.hpp"
#include "material.hpp"

class Shape
{
public:
    Shape(const std::string &name)
        : name(name), transform(4)
    {
        transform.SetIdentity();
    }
    virtual ~Shape() = default;

    Material &GetMutableMaterial() { return material; }
    const Material &GetMaterial() const { return material; }
    void SetMaterial(const Material &m) { material = m; }

    const Matrix &GetTransform() const { return transform; }
    Matrix &GetMutableTransform() { return transform; }
    void SetTransform(const Matrix &t) { transform = t; }

    const std::string &GetName() const { return name; }

    Tuple NormalAt(const Tuple &point) const;

protected:
    std::string name;
    Matrix transform;  // The transformation matrix for the sphere (default is identity)
    Material material; // The material properties of the sphere
};
