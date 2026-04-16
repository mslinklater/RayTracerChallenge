#pragma once
#include "color.hpp"
#include <cassert>
#include <cmath>
#include <memory>
#include <type_traits>

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

    Material(const Material &other);

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
    const Pattern *GetPattern() const
    {
        return pattern.get();
    }

    /** @brief Returns a mutable pointer to the optional surface pattern, or nullptr. */
    Pattern *GetMutablePattern()
    {
        return pattern.get();
    }

    /** @brief Sets the base surface colour. */
    Material &SetColor(const Color &c)
    {
        assert(std::isfinite(c.r));
        assert(std::isfinite(c.g));
        assert(std::isfinite(c.b));
        color = c;
        return *this;
    }

    /** @brief Sets the ambient reflection coefficient. */
    Material &SetAmbient(float a)
    {
        assert(std::isfinite(a));
        assert(a >= 0.f && a <= 1.f);
        ambient = a;
        return *this;
    }

    /** @brief Sets the diffuse reflection coefficient. */
    Material &SetDiffuse(float d)
    {
        assert(std::isfinite(d));
        assert(d >= 0.f && d <= 1.f);
        diffuse = d;
        return *this;
    }

    /** @brief Sets the specular reflection coefficient. */
    Material &SetSpecular(float s)
    {
        assert(std::isfinite(s));
        assert(s >= 0.f && s <= 1.f);
        specular = s;
        return *this;
    }

    /** @brief Sets the shininess exponent used for specular highlights. */
    Material &SetShininess(float sh)
    {
        assert(std::isfinite(sh));
        assert(sh >= 0.f);
        shininess = sh;
        return *this;
    }

    /** @brief Sets the reflectivity (0 = non-reflective, 1 = mirror). */
    Material &SetReflective(float r)
    {
        assert(std::isfinite(r));
        assert(r >= 0.f && r <= 1.f);
        reflective = r;
        return *this;
    }

    /** @brief Sets the transparency (0 = opaque, 1 = fully transparent). */
    Material &SetTransparency(float t)
    {
        assert(std::isfinite(t));
        assert(t >= 0.f && t <= 1.f);
        transparency = t;
        return *this;
    }

    /** @brief Sets the refractive index (1.0 for vacuum/air). */
    Material &SetRefractiveIndex(float ri)
    {
        assert(std::isfinite(ri));
        assert(ri > 0.f);
        refractiveIndex = ri;
        return *this;
    }

    /**
     * @brief Attaches a copy of @p object as the surface pattern.
     * @tparam T A concrete subclass of @c Pattern.
     * @param object The pattern instance to store.
     */
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<Pattern, T>>> void SetPattern(const T &object)
    {
        assert(object.GetTransform().GetSize() == 4);
        assert(object.GetTransform().IsValid());
        pattern = std::make_shared<T>(object);
    }

    /** @brief Copy-assigns all shading parameters and the optional pattern. */
    Material &operator=(const Material &other);

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
