#include "material.hpp"
#include "maths.hpp"
#include "pattern.hpp"
#include <cassert>
#include <cmath>

namespace
{
bool IsFiniteColor(const Color &color)
{
    return std::isfinite(color.r) && std::isfinite(color.g) && std::isfinite(color.b);
}

bool IsValidMaterialState(const Material &material)
{
    return IsFiniteColor(material.GetColor()) && std::isfinite(material.GetAmbient()) &&
           std::isfinite(material.GetDiffuse()) && std::isfinite(material.GetSpecular()) &&
           std::isfinite(material.GetShininess()) && std::isfinite(material.GetReflective()) &&
           std::isfinite(material.GetTransparency()) && std::isfinite(material.GetRefractiveIndex()) &&
           material.GetAmbient() >= 0.f && material.GetAmbient() <= 1.f &&
           material.GetDiffuse() >= 0.f && material.GetDiffuse() <= 1.f &&
           material.GetSpecular() >= 0.f && material.GetSpecular() <= 1.f &&
           material.GetShininess() >= 0.f && material.GetReflective() >= 0.f && material.GetReflective() <= 1.f &&
           material.GetTransparency() >= 0.f && material.GetTransparency() <= 1.f &&
           material.GetRefractiveIndex() > 0.f;
}
}

Material::Material(const Material &other)
{
    *this = other;
}

Material &Material::operator=(const Material &other)
{
    assert(IsValidMaterialState(other));
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
    assert(IsValidMaterialState(lhs));
    assert(IsValidMaterialState(rhs));
    return lhs.GetColor() == rhs.GetColor() && AreEqual(lhs.GetAmbient(), rhs.GetAmbient()) &&
           AreEqual(lhs.GetDiffuse(), rhs.GetDiffuse()) && AreEqual(lhs.GetSpecular(), rhs.GetSpecular()) &&
           AreEqual(lhs.GetShininess(), rhs.GetShininess()) && AreEqual(lhs.GetReflective(), rhs.GetReflective()) &&
           AreEqual(lhs.GetTransparency(), rhs.GetTransparency()) &&
           AreEqual(lhs.GetRefractiveIndex(), rhs.GetRefractiveIndex()) &&
           ((lhs.GetPattern() == nullptr && rhs.GetPattern() == nullptr) ||
            (lhs.GetPattern() != nullptr && rhs.GetPattern() != nullptr && *lhs.GetPattern() == *rhs.GetPattern()));
}
