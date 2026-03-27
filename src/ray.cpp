#include "ray.hpp"
#include "matrix.hpp"

// Tuple Position(const Ray &ray, float t)
//{
//     return ray.GetOrigin() + ray.GetDirection() * t;
// }

Ray operator*(const Ray &ray, const Matrix &matrix)
{
    return Ray(matrix * ray.GetOrigin(), matrix * ray.GetDirection());
}
