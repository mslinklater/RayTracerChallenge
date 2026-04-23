#include "material.hpp"
#include "maths.hpp"
#include "pattern.hpp"
#include <cassert>
#include <cmath>

Material::Material()
{
}

Material::Material(const Material& other)
{
    *this = other;
}

Material& Material::operator=(const Material& other)
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

bool operator==(const Material& lhs, const Material& rhs)
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

Color Material::GetColor() const
{
    return color;
}

float Material::GetAmbient() const
{
    return ambient;
}

float Material::GetDiffuse() const
{
    return diffuse;
}

float Material::GetSpecular() const
{
    return specular;
}

float Material::GetShininess() const
{
    return shininess;
}

float Material::GetReflective() const
{
    return reflective;
}

float Material::GetTransparency() const
{
    return transparency;
}

float Material::GetRefractiveIndex() const
{
    return refractiveIndex;
}

const Pattern* Material::GetPattern() const
{
    return pattern.get();
}

Pattern* Material::GetMutablePattern()
{
    return pattern.get();
}

Material& Material::SetColor(const Color& c)
{
    assert(std::isfinite(c.r));
    assert(std::isfinite(c.g));
    assert(std::isfinite(c.b));
    color = c;
    return *this;
}

Material& Material::SetAmbient(float a)
{
    assert(std::isfinite(a));
    assert(a >= 0.f && a <= 1.f);
    ambient = a;
    return *this;
}

Material& Material::SetDiffuse(float d)
{
    assert(std::isfinite(d));
    assert(d >= 0.f && d <= 1.f);
    diffuse = d;
    return *this;
}

Material& Material::SetSpecular(float s)
{
    assert(std::isfinite(s));
    assert(s >= 0.f && s <= 1.f);
    specular = s;
    return *this;
}

Material& Material::SetShininess(float sh)
{
    assert(std::isfinite(sh));
    assert(sh >= 0.f);
    shininess = sh;
    return *this;
}

Material& Material::SetReflective(float r)
{
    assert(std::isfinite(r));
    assert(r >= 0.f && r <= 1.f);
    reflective = r;
    return *this;
}

Material& Material::SetTransparency(float t)
{
    assert(std::isfinite(t));
    assert(t >= 0.f && t <= 1.f);
    transparency = t;
    return *this;
}

Material& Material::SetRefractiveIndex(float ri)
{
    assert(std::isfinite(ri));
    assert(ri > 0.f);
    refractiveIndex = ri;
    return *this;
}

bool Material::IsValid() const
{
    return color.IsValid() && std::isfinite(ambient) && std::isfinite(diffuse) && std::isfinite(specular) &&
           std::isfinite(shininess) && std::isfinite(reflective) && std::isfinite(transparency) &&
           std::isfinite(refractiveIndex) && ambient >= 0.f && ambient <= 1.f && diffuse >= 0.f && diffuse <= 1.f &&
           specular >= 0.f && specular <= 1.f && shininess >= 0.f && reflective >= 0.f && reflective <= 1.f &&
           transparency >= 0.f && transparency <= 1.f && refractiveIndex > 0.f;
}
