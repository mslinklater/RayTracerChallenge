#include "shape.hpp"
#include "tuple.hpp"

// TODO - remove these shit hacks
Ray gSavedRay; // Global variable to store the ray for testing purposes
    
Tuple Shape::NormalAt(const Tuple &point) const
{
    Tuple objectPoint = transform.GetInverse() * point;

    Tuple objectNormal = NormalAtLocal(objectPoint);

    Tuple worldNormal = transform.GetInverse().Transpose() * objectNormal;
    worldNormal.w = 0.f; // Ensure it's a vector
    return worldNormal.Normalize();
}

Tuple Shape::NormalAtLocal(const Tuple &point) const
{
    return Vector(point.x, point.y, point.z);
}

std::vector<float> Shape::Intersect(const Ray &ray) const
{
    gSavedRay = ray * transform.GetInverse();

    return IntersectLocal(gSavedRay);
}

std::vector<float> Shape::IntersectLocal(const Ray &ray) const
{
    return std::vector<float>(); // Default implementation returns an empty vector (no intersections)
}