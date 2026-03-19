#pragma once
#include "matrix.hpp"
#include "material.hpp"

class Sphere
{
public:
    Sphere() : transform(4)
    {
        transform.SetIdentity();
    }

    Tuple NormalAt(const Tuple &point) const;

    Material &GetMutableMaterial() { return material; }
    const Material &GetMaterial() const { return material; }
    void SetMaterial(const Material &m) { material = m; }

    const Matrix &GetTransform() const { return transform; }
    Matrix &GetMutableTransform() { return transform; }
    void SetTransform(const Matrix &t) { transform = t; }

private:
    Matrix transform;  // The transformation matrix for the sphere (default is identity)
    Material material; // The material properties of the sphere
};

bool operator==(const Sphere &s1, const Sphere &s2);