#include "material.hpp"
#include "maths.hpp"
#include "pattern.hpp"
#include <cassert>
#include <cmath>

Material::Material(const Material &other)
{
    *this = other;
}

Material &Material::operator=(const Material &other)
{
    assert(other.IsValid());

    if (this != &other)
    {
        ambient = other.ambient;
        diffuse = other.diffuse;
        specular = other.specular;
        shininess = other.shininess;
        color = other.color;
        reflective = other.reflective;
        transparency = other.transparency;
        refractiveIndex = other.refractiveIndex;
        if (other.pattern)
        {
            pattern = other.pattern->Clone();
        }
        else
        {
            pattern = nullptr;
        }
    }
    return *this;
}

bool operator==(const Material &lhs, const Material &rhs)
{
    assert(lhs.IsValid());
    assert(rhs.IsValid());
    return lhs.GetColor() == rhs.GetColor() && AreEqual(lhs.GetAmbient(), rhs.GetAmbient()) &&
           AreEqual(lhs.GetDiffuse(), rhs.GetDiffuse()) && AreEqual(lhs.GetSpecular(), rhs.GetSpecular()) &&
           AreEqual(lhs.GetShininess(), rhs.GetShininess()) && AreEqual(lhs.GetReflective(), rhs.GetReflective()) &&
           AreEqual(lhs.GetTransparency(), rhs.GetTransparency()) &&
           AreEqual(lhs.GetRefractiveIndex(), rhs.GetRefractiveIndex()) &&
           ((lhs.GetPattern() == nullptr && rhs.GetPattern() == nullptr) ||
            (lhs.GetPattern() != nullptr && rhs.GetPattern() != nullptr && *lhs.GetPattern() == *rhs.GetPattern()));
}
