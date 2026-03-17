#include "ray.hpp"
#include "tuple.hpp"
#include "matrix.hpp"

Tuple Position(const Ray &ray, float t)
{
    return ray.GetOrigin() + ray.GetDirection() * t;
}

std::vector<float> Intersect(const Sphere &sphere, const Ray &ray)
{
    Ray transformedRay = ray * sphere.GetTransform().GetInverse();

    // For a sphere centered at the origin with radius 1, the intersection can be calculated using the quadratic formula.
    // The coefficients of the quadratic equation are derived from substituting the ray equation into the sphere equation.
    Tuple sphereToRay = transformedRay.GetOrigin() - Point(0.f, 0.f, 0.f);   // Since the sphere is at the origin, we can use the ray's origin directly
    float a = transformedRay.GetDirection() | transformedRay.GetDirection(); // Dot product of direction with itself
    float b = 2.f * (transformedRay.GetDirection() | sphereToRay);           // 2 times the dot product of direction and origin
    float c = (sphereToRay | sphereToRay) - 1.f;                             // Dot product of origin with itself minus radius squared

    float discriminant = b * b - 4.f * a * c;

    std::vector<float> intersections;
    if (discriminant < 0.f)
    {
        // No intersections
        return intersections;
    }
    else
    {
        float sqrtDiscriminant = std::sqrt(discriminant);
        float t1 = (-b - sqrtDiscriminant) / (2.f * a);
        float t2 = (-b + sqrtDiscriminant) / (2.f * a);
        intersections.push_back(t1);
        intersections.push_back(t2);
        return intersections;
    }
}

Ray operator*(const Ray &ray, const Matrix &matrix)
{
    return Ray(matrix * ray.GetOrigin(), matrix * ray.GetDirection());
}
