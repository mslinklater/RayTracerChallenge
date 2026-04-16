#pragma once
#include "tuple.hpp"
#include "color.hpp"
#include <cassert>
#include <cmath>

/**
 * @brief A point light source with a position and an intensity (colour).
 *
 * The renderer supports multiple lights per world; each contributes to the
 * Phong shading calculation independently.
 */
struct Light
{
    /**
     * @brief Constructs a point light.
     * @param position  World-space position of the light source.
     * @param intensity Colour and brightness of the emitted light.
     */
    Light(const Tuple &position, const Color &intensity) : position(position), intensity(intensity)
    {
        assert(position.IsValid());
        assert(position.IsPoint());
        assert(std::isfinite(intensity.r));
        assert(std::isfinite(intensity.g));
        assert(std::isfinite(intensity.b));
    }

    Tuple position;  ///< World-space position of the light source.
    Color intensity; ///< Colour and brightness of the emitted light.

    /** @brief Equality comparison. */
    bool operator==(const Light &other) const;
};
