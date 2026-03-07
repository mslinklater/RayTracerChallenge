#include "ray.hpp"

Tuple Position(const Ray &ray, float t)
{
    return ray.origin + ray.direction * t;
}
