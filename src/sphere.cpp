#include "sphere.hpp"

Tuple Sphere::NormalAt(const Tuple &point) const
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

bool operator==(const Sphere &s1, const Sphere &s2)
{
    return s1.GetTransform() == s2.GetTransform() && s1.GetMaterial() == s2.GetMaterial() && s1.GetName() == s2.GetName();
}