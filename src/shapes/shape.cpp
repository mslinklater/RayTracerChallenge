#include "shapes/shape.hpp"
#include "tuple.hpp"
#include <cassert>

Shape::Shape(const std::string& name) : name(name), worldObjectId(kInvalidObjectId), transform(4)
{
    assert(!name.empty());
    transform.SetIdentity();
}

Shape::Shape(const Shape& other)
    : worldObjectId(other.worldObjectId), name(other.name), transform(other.transform), material(other.material),
      inverseTransform(4), inverseTransposeTransform(4)
{
    UpdateTransformCache();
}

Shape& Shape::operator=(const Shape& other)
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

void Shape::SetWorldObjectId(ObjectId id)
{
    assert(id != kInvalidObjectId);
    worldObjectId = id;
}

ObjectId Shape::GetWorldObjectId() const
{
    return worldObjectId;
}

Material& Shape::GetMutableMaterial()
{
    return material;
}

const Material& Shape::GetMaterial() const
{
    return material;
}

void Shape::SetMaterial(const Material& m)
{
    material = m;
}

const Matrix& Shape::GetTransform() const
{
    return transform;
}

Matrix& Shape::GetMutableTransform()
{
    transformCacheValidAtomic = false;
    return transform;
}

const std::string& Shape::GetName() const
{
    return name;
}

Shape* Shape::GetParent() const
{
    return parent;
}

void Shape::SetParent(Shape* parent)
{
    assert(parent != nullptr);
    if (this->GetParent() != nullptr && this->GetParent() != parent)
    {
        throw std::invalid_argument("Group::SetParent() Shape '" + this->GetName() +
                                    "' already has a different parent.");
    }
    this->parent = parent;
}

void Shape::SetTransform(const Matrix& t)
{
    assert(t.GetSize() == 4);
    assert(t.IsValid());
    transform = t;
    UpdateTransformCache();
}

void Shape::UpdateTransformCache() const
{
    assert(transform.GetSize() == 4);
    assert(transform.IsValid());

    inverseTransform = transform.GetInverse();
    inverseTransposeTransform = inverseTransform.Transpose();
    transformCacheValidAtomic.store(true, std::memory_order_release);
}

void Shape::EnsureTransformCache() const
{
    if (!transformCacheValidAtomic.load(std::memory_order_acquire))
    {
        std::lock_guard<std::mutex> lock(transformCacheMutex);
        if (!transformCacheValidAtomic.load(std::memory_order_relaxed))
        {
            UpdateTransformCache();
        }
    }
}

Tuple Shape::NormalAt(const Tuple& point) const
{
    assert(point.IsValid());

    const Tuple objectPoint = WorldToObject(point);

    const Tuple objectNormal = NormalAtLocal(objectPoint);

    Tuple worldNormal = inverseTransposeTransform * objectNormal;
    worldNormal.w = 0.f; // Ensure it's a vector
    return worldNormal.Normalize();
}

Tuple Shape::WorldToObject(const Tuple& point) const
{
    assert(point.IsValid());

    EnsureTransformCache();
    return inverseTransform * point;
}

Tuple Shape::NormalAtLocal(const Tuple& point) const
{
    assert(point.IsValid());

    return Vector(point.x, point.y, point.z);
}

std::vector<float> Shape::Intersect(const Ray& ray) const
{
    assert(ray.IsValid());

    EnsureTransformCache();
    const Ray localRay = ray * inverseTransform;
    return IntersectLocal(localRay);
}

std::vector<float> Shape::IntersectLocal(const Ray& ray) const
{
    assert(ray.IsValid());

    return std::vector<float>(); // Default implementation returns an empty vector (no intersections)
}
