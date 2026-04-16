#include "ray.hpp"
#include "matrix.hpp"
#include <cassert>

Ray operator*(const Ray &ray, const Matrix &matrix)
{
    assert(ray.GetOrigin().IsValid());
    assert(ray.GetDirection().IsValid());
    assert(matrix.GetSize() == 4);
    assert(matrix.IsValid());
    return Ray(matrix * ray.GetOrigin(), matrix * ray.GetDirection());
}
