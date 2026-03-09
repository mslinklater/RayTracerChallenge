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

    Tuple NormalAt(const Tuple &point) const
    {
        // To find the normal at a point on the sphere, we first need to transform the point to object space using the inverse of the sphere's transformation matrix.
        Tuple objectPoint = transform.GetInverse() * point;
        // The normal in object space is simply the vector from the center of the sphere (which is at the origin) to the point.
        Tuple objectNormal = objectPoint - Point(0.f, 0.f, 0.f);
        // We then need to transform the normal back to world space using the transpose of the inverse of the sphere's transformation matrix.
        Tuple worldNormal = transform.GetInverse().Transpose() * objectNormal;
        worldNormal.w = 0.f; // Ensure it's a vector
        return worldNormal.Normalize();
    }

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