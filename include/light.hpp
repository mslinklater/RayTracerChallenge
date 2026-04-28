#pragma once
#include "color.hpp"
#include "tuple.hpp"
#include <cassert>

/**
 * @brief A point light source with a position and an intensity (colour).
 *
 * The renderer supports multiple lights per world; each contributes to the
 * Phong shading calculation independently.
 */
class Light
{
  public:
    /**
     * @brief Constructs a point light.
     * @param position  World-space position of the light source.
     * @param intensity Colour and brightness of the emitted light.
     */
    Light(const Tuple& position, const Color& intensity);

    /** @brief return the intensity of the light. */
    const Color& GetIntensity() const;

    /** @brief sets the intensity of the light. */
    Light& SetIntensity(const Color& intensity);

    /** @brief return the position of the light. */
    const Tuple& GetPosition() const;

    /** @brief sets the position of the light. */
    Light& SetPosition(const Tuple& position);

    /** @brief Equality comparison. */
    bool operator==(const Light& other) const;

    /** @brief returns whether the lighy has valid values. */
    bool IsValid() const;

  private:
    Tuple position;  ///< World-space position of the light source.
    Color intensity; ///< Colour and brightness of the emitted light.
};
