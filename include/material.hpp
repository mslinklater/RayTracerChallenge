#pragma once
#include "color.hpp"
#include <memory>

class Pattern;

/**
 * @brief Phong shading parameters and optional texture pattern for a surface.
 *
 * All lighting coefficients default to physically plausible starting values.
 * An optional @c Pattern overrides the base @c color when present.
 */
class Material
{
  public:
    /// @name Default shading parameter values
    /// @{
    constexpr static Color kDefaultColor = kColorWhite;   ///< Default surface colour (white).
    constexpr static float kDefaultAmbient = 0.1f;        ///< Default ambient coefficient.
    constexpr static float kDefaultDiffuse = 0.9f;        ///< Default diffuse coefficient.
    constexpr static float kDefaultSpecular = 0.9f;       ///< Default specular coefficient.
    constexpr static float kDefaultShininess = 200.f;     ///< Default specular shininess exponent.
    constexpr static float kDefaultReflective = 0.f;      ///< Default reflectivity (0 = non-reflective).
    constexpr static float kDefaultTransparency = 0.f;    ///< Default transparency (0 = opaque).
    constexpr static float kDefaultRefractiveIndex = 1.f; ///< Default refractive index (vacuum).
    /// @}

    /** @brief Constructs a material with all default shading parameters. */
    Material()
    {
    }

    /** @brief Returns the base surface colour. */
    Color GetColor() const
    {
        return color;
    }
    /** @brief Returns the ambient reflection coefficient. */
    float GetAmbient() const
    {
        return ambient;
    }
    /** @brief Returns the diffuse reflection coefficient. */
    float GetDiffuse() const
    {
        return diffuse;
    }
    /** @brief Returns the specular reflection coefficient. */
    float GetSpecular() const
    {
        return specular;
    }
    /** @brief Returns the shininess exponent used for specular highlights. */
    float GetShininess() const
    {
        return shininess;
    }
    /** @brief Returns the reflectivity of the material (0 = non-reflective, 1 = mirror). */
    float GetReflective() const
    {
        return reflective;
    }
    /** @brief Returns the transparency of the material (0 = opaque, 1 = fully transparent). */
    float GetTransparency() const
    {
        return transparency;
    }
    /** @brief Returns the refractive index (1.0 for vacuum/air). */
    float GetRefractiveIndex() const
    {
        return refractiveIndex;
    }

    /**
     * @brief Returns a raw pointer to the optional surface pattern, or nullptr.
     *
     * The pattern, when present, replaces the base colour at each surface point.
     */
    Pattern *GetPattern() const
    {
        return pattern.get();
    }

    /** @brief Sets the base surface colour. */
    void SetColor(const Color &c)
    {
        color = c;
    }

    /** @brief Sets the ambient reflection coefficient. */
    void SetAmbient(float a)
    {
        ambient = a;
    }

    /** @brief Sets the diffuse reflection coefficient. */
    void SetDiffuse(float d)
    {
        diffuse = d;
    }

    /** @brief Sets the specular reflection coefficient. */
    void SetSpecular(float s)
    {
        specular = s;
    }

    /** @brief Sets the shininess exponent used for specular highlights. */
    void SetShininess(float sh)
    {
        shininess = sh;
    }

    /** @brief Sets the reflectivity (0 = non-reflective, 1 = mirror). */
    void SetReflective(float r)
    {
        reflective = r;
    }

    /** @brief Sets the transparency (0 = opaque, 1 = fully transparent). */
    void SetTransparency(float t)
    {
        transparency = t;
    }

    /** @brief Sets the refractive index (1.0 for vacuum/air). */
    void SetRefractiveIndex(float ri)
    {
        refractiveIndex = ri;
    }

    /**
     * @brief Attaches a copy of @p object as the surface pattern.
     * @tparam T A concrete subclass of @c Pattern.
     * @param object The pattern instance to store.
     */
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<Pattern, T>>> void SetPattern(T &object)
    {
        pattern = std::make_shared<T>(object);
    }

    /** @brief Copy-assigns all shading parameters and the optional pattern. */
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
    Color color = kDefaultColor;                     ///< The base color of the material.
    float ambient = kDefaultAmbient;                 ///< The ambient reflection coefficient.
    float diffuse = kDefaultDiffuse;                 ///< The diffuse reflection coefficient.
    float specular = kDefaultSpecular;               ///< The specular reflection coefficient.
    float shininess = kDefaultShininess;             ///< The shininess factor for specular highlights.
    float reflective = kDefaultReflective;           ///< The reflectivity of the material.
    float transparency = kDefaultTransparency;       ///< The transparency of the material.
    float refractiveIndex = kDefaultRefractiveIndex; ///< The refractive index of the material.
    std::shared_ptr<Pattern> pattern = nullptr;      ///< Optional pattern for the material.
};

/** @brief Equality comparison using floating-point tolerance on all fields. */
bool operator==(const Material &lhs, const Material &rhs);
