#include "sphere.hpp"

bool operator==(const Sphere &s1, const Sphere &s2)
{
    return s1.GetTransform() == s2.GetTransform() && s1.GetMaterial() == s2.GetMaterial() && s1.GetName() == s2.GetName();
}