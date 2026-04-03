#pragma once
#include "color.hpp"
#include "pattern.hpp"
#include <memory>

class Material
{
  public:
    Material() : color(1.f, 1.f, 1.f), ambient(0.1f), diffuse(0.9f), specular(0.9f), shininess(200.f)
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
    StripePattern *GetPattern() const
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
    void SetPattern(const StripePattern &p)
    {
        pattern = std::make_shared<StripePattern>(p);
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
    Color color;                                      // The base color of the material
    float ambient;                                    // The ambient reflection coefficient
    float diffuse;                                    // The diffuse reflection coefficient
    float specular;                                   // The specular reflection coefficient
    float shininess;                                  // The shininess factor for specular highlights
    std::shared_ptr<StripePattern> pattern = nullptr; // Optional pattern for the material
};

bool operator==(const Material &lhs, const Material &rhs);
