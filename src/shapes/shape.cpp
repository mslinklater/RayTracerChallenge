#include "shapes/shape.hpp"
#include "tuple.hpp"

void Shape::UpdateTransformCache() const
{
    inverseTransform = transform.GetInverse();
    inverseTransposeTransform = inverseTransform.Transpose();
    transformCacheValid = true;
}

void Shape::EnsureTransformCache() const
{
    if (!transformCacheValid)
    {
        UpdateTransformCache();
    }
}

Tuple Shape::NormalAt(const Tuple &point) const
{
    Tuple objectPoint = WorldToObject(point);

    Tuple objectNormal = NormalAtLocal(objectPoint);

    Tuple worldNormal = inverseTransposeTransform * objectNormal;
    worldNormal.w = 0.f; // Ensure it's a vector
    return worldNormal.Normalize();
}

Tuple Shape::WorldToObject(const Tuple &point) const
{
    EnsureTransformCache();
    return inverseTransform * point;
}

Tuple Shape::NormalAtLocal(const Tuple &point) const
{
    return Vector(point.x, point.y, point.z);
}

std::vector<float> Shape::Intersect(const Ray &ray) const
{
    EnsureTransformCache();
    Ray localRay = ray * inverseTransform;
    return IntersectLocal(localRay);
}

std::vector<float> Shape::IntersectLocal(const Ray &ray) const
{
    return std::vector<float>(); // Default implementation returns an empty vector (no intersections)
}
