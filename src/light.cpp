#include "light.hpp"
#include "material.hpp"
#include "tuple.hpp"

Color Lighting(const Material &material, const Light &light, const Tuple &position, const Tuple &eyeVector, const Tuple &normalVector)
{
    Color effectiveColor = material.GetColor() * light.intensity;
    Tuple lightVector = (light.position - position).Normalize();

    // the three components of the Phong reflection model: ambient, diffuse, and specular
    Color ambient = effectiveColor * material.GetAmbient();
    Color diffuse;
    Color specular;

    float lightDotNormal = lightVector | normalVector;

    if (lightDotNormal < 0.f)
    {
        diffuse = Color(0.f, 0.f, 0.f);
        specular = Color(0.f, 0.f, 0.f);
    }
    else
    {
        diffuse = effectiveColor * material.GetDiffuse() * lightDotNormal;

        Tuple reflectVector = (-lightVector).Reflect(normalVector);
        float reflectDotEye = reflectVector | eyeVector;

        if (reflectDotEye <= 0.f)
        {
            specular = Color(0.f, 0.f, 0.f);
        }
        else
        {
            float factor = std::pow(reflectDotEye, material.GetShininess());
            specular = light.intensity * material.GetSpecular() * factor;
        }
    }

    return ambient + diffuse + specular;
}