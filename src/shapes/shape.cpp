#include "shapes/shape.hpp"
#include "tuple.hpp"

Shape::Shape(const Shape &other)
    : worldObjectId(other.worldObjectId), name(other.name), transform(other.transform), material(other.material),
      inverseTransform(4), inverseTransposeTransform(4)
{
    UpdateTransformCache();
}

Shape &Shape::operator=(const Shape &other)
{
    if (this != &other)
    {
        worldObjectId = other.worldObjectId;
        name = other.name;
        transform = other.transform;
        material = other.material;
        UpdateTransformCache();
    }
    return *this;
}

void Shape::UpdateTransformCache() const
{
    inverseTransform = transform.GetInverse();
    inverseTransposeTransform = inverseTransform.Transpose();
    transformCacheValid.store(true, std::memory_order_release);
}

void Shape::EnsureTransformCache() const
{
    if (!transformCacheValid.load(std::memory_order_acquire))
    {
        std::lock_guard<std::mutex> lock(transformCacheMutex);
        if (!transformCacheValid.load(std::memory_order_relaxed))
        {
            UpdateTransformCache();
        }
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
