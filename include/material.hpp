#pragma once
#include "color.hpp"
#include <memory>

class Pattern;

class Material
{
  public:
    constexpr static Color kDefaultColor = kColorWhite;
    constexpr static float kDefaultAmbient = 0.1f;
    constexpr static float kDefaultDiffuse = 0.9f;
    constexpr static float kDefaultSpecular = 0.9f;
    constexpr static float kDefaultShininess = 200.f;
    constexpr static float kDefaultReflective = 0.f;
    constexpr static float kDefaultTransparency = 0.f;
    constexpr static float kDefaultRefractiveIndex = 1.f;

    Material()
    {
    }

    Color GetColor() const
    {
        return color;
    }
    float GetAmbient() const
    {
        return ambient;
    }
    float GetDiffuse() const
    {
        return diffuse;
    }
    float GetSpecular() const
    {
        return specular;
    }
    float GetShininess() const
    {
        return shininess;
    }
    float GetReflective() const
    {
        return reflective;
    }
    float GetTransparency() const
    {
        return transparency;
    }
    float GetRefractiveIndex() const
    {
        return refractiveIndex;
    }

    Pattern *GetPattern() const
    {
        return pattern.get();
    }

    void SetColor(const Color &c)
    {
        color = c;
    }
    void SetAmbient(float a)
    {
        ambient = a;
    }
    void SetDiffuse(float d)
    {
        diffuse = d;
    }
    void SetSpecular(float s)
    {
        specular = s;
    }
    void SetShininess(float sh)
    {
        shininess = sh;
    }
    void SetReflective(float r)
    {
        reflective = r;
    }
    void SetTransparency(float t)
    {
        transparency = t;
    }
    void SetRefractiveIndex(float ri)
    {
        refractiveIndex = ri;
    }

    template <typename T, typename = std::enable_if_t<std::is_base_of_v<Pattern, T>>> void SetPattern(T &object)
    {
        pattern = std::make_shared<T>(object);
    }

    // Copy assignment operator
    Material &operator=(const Material &other)
    {
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
                pattern = other.pattern; //->Clone();
            }
            else
            {
                pattern = nullptr;
            }
        }
        return *this;
    }

  private:
    Color color = kDefaultColor;                     // The base color of the material
    float ambient = kDefaultAmbient;                 // The ambient reflection coefficient
    float diffuse = kDefaultDiffuse;                 // The diffuse reflection coefficient
    float specular = kDefaultSpecular;               // The specular reflection coefficient
    float shininess = kDefaultShininess;             // The shininess factor for specular highlights
    float reflective = kDefaultReflective;           // The reflectivity of the material
    float transparency = kDefaultTransparency;       // The transparency of the material
    float refractiveIndex = kDefaultRefractiveIndex; // The refractive index of the material
    std::shared_ptr<Pattern> pattern = nullptr;      // Optional pattern for the material
};

bool operator==(const Material &lhs, const Material &rhs);
