#pragma once
#include "material.hpp"
#include "matrix.hpp"
#include "ray.hpp"
#include "types.hpp"
#include <string>

extern Ray gSavedRay;
extern Tuple gSavedNormal;

class Shape
{
  public:
    Shape(const std::string &name) : name(name), worldObjectId(kInvalidObjectId), transform(4)
    {
        transform.SetIdentity();
    }
    virtual ~Shape() = default;

    Material &GetMutableMaterial()
    {
        return material;
    }
    const Material &GetMaterial() const
    {
        return material;
    }
    void SetMaterial(const Material &m)
    {
        material = m;
    }

    const Matrix &GetTransform() const
    {
        return transform;
    }
    Matrix &GetMutableTransform()
    {
        return transform;
    }
    void SetTransform(const Matrix &t)
    {
        transform = t;
    }

    const std::string &GetName() const
    {
        return name;
    }

    Tuple NormalAt(const Tuple &point) const;
    virtual Tuple NormalAtLocal(const Tuple &point) const;

    std::vector<float> Intersect(const Ray &ray) const;              // intersect a ray which is in global space
    virtual std::vector<float> IntersectLocal(const Ray &ray) const; // intersect a ray which is in local space

  protected:
    ObjectId worldObjectId = kInvalidObjectId; /// ObjectId which is unique

    std::string name;
    Matrix transform;  // The transformation matrix for the sphere (default is identity)
    Material material; // The material properties of the sphere
};
