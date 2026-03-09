#pragma once
#include "color.hpp"

class Material
{
public:
    Material() : color(1.f, 1.f, 1.f), ambient(0.1f), diffuse(0.9f), specular(0.9f), shininess(200.f) {}

    Color GetColor() const { return color; }
    float GetAmbient() const { return ambient; }
    float GetDiffuse() const { return diffuse; }
    float GetSpecular() const { return specular; }
    float GetShininess() const { return shininess; }

    void SetColor(const Color &c) { color = c; }
    void SetAmbient(float a) { ambient = a; }
    void SetDiffuse(float d) { diffuse = d; }
    void SetSpecular(float s) { specular = s; }
    void SetShininess(float sh) { shininess = sh; }

private:
    Color color;     // The base color of the material
    float ambient;   // The ambient reflection coefficient
    float diffuse;   // The diffuse reflection coefficient
    float specular;  // The specular reflection coefficient
    float shininess; // The shininess factor for specular highlights
};
