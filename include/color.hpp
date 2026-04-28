#pragma once
#include <iostream>

/**
 * @brief An RGB colour value with floating-point channels in the range [0, 1].
 */
struct Color
{
    float r = 0.f; ///< Red channel.
    float g = 0.f; ///< Green channel.
    float b = 0.f; ///< Blue channel.

    /** @brief Adds two colours component-wise. */
    Color operator+(const Color& other) const;
    /** @brief Subtracts @p other from this colour component-wise. */
    Color operator-(const Color& other) const;
    /** @brief Scales all channels by @p scalar. */
    Color operator*(const float scalar) const;
    /** @brief Multiplies two colours component-wise (Hadamard product). */
    Color operator*(const Color& other) const;
    /** @brief Equality comparison using floating-point tolerance. */
    bool operator==(const Color& other) const;

    /** @brief Returns whether the Color has valid values */
    bool IsValid() const
    {
        return std::isfinite(r) && std::isfinite(g) && std::isfinite(b);
    }
};

/// @brief Colour returned when a ray misses all geometry (cyan sentinel).
static constexpr Color kBackgroundColor(0.f, 1.f, 1.f);

/// @name Predefined colour constants
/// @{
static constexpr Color kColorBlack(0.f, 0.f, 0.f);
static constexpr Color kColorRed(1.f, 0.f, 0.f);
static constexpr Color kColorGreen(0.f, 1.f, 0.f);
static constexpr Color kColorBlue(0.f, 0.f, 1.f);
static constexpr Color kColorYellow(1.f, 1.f, 0.f);
static constexpr Color kColorMagenta(1.f, 0.f, 1.f);
static constexpr Color kColorCyan(0.f, 1.f, 1.f);
static constexpr Color kColorWhite(1.f, 1.f, 1.f);
/// @}
