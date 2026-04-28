#pragma once
#include "tuple.hpp"
#include "types.hpp"

/**
 * @brief Pre-computed shading data derived from a ray-object intersection.
 *
 * Populated by @c Renderer::PrepareComputations and consumed by
 * @c Renderer::ShadeHit to avoid recalculating geometry per-light.
 */
struct Computations // TODO: This needs renaming
{
    float t = 0.f;                             ///< Ray parameter at the intersection point.
    ObjectId objectId = kInvalidObjectId;      ///< World ID of the intersected shape.
    Tuple point = {0.f, 0.f, 0.f, 1.f};        ///< World-space intersection point.
    Tuple overPoint = {0.f, 0.f, 0.f, 1.f};    ///< Point offset slightly above the surface (prevents self-shadowing).
    Tuple underPoint = {0.f, 0.f, 0.f, 1.f};   ///< Point offset slightly above the surface (prevents self-shadowing).
    Tuple eyeVector = {0.f, 0.f, 0.f, 0.f};    ///< Direction from the intersection back toward the eye.
    Tuple normalVector = {0.f, 0.f, 0.f, 0.f}; ///< Surface normal at the intersection, pointing toward the eye.
    Tuple reflectv = {0.f, 0.f, 0.f, 0.f};     ///< Reflection direction of the incoming ray about the surface normal.
    bool inside = false;                       ///< True when the intersection occurs on the inside of the object.
    float n1 = 0.f;                            ///< Refractive index of the material the ray is exiting.
    float n2 = 0.f;                            ///< Refractive index of the material the ray is entering.

    bool IsValid() const
    {
        return std::isfinite(t) && objectId != kInvalidObjectId && point.IsValid() && overPoint.IsValid() &&
               underPoint.IsValid() && eyeVector.IsValid() && normalVector.IsValid() && reflectv.IsValid();
    }
};
