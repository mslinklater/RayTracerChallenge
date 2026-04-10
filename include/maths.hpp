#pragma once

#include <cmath>

/// @brief Conversion factor: multiply degrees by this to obtain radians.
static constexpr float kDegreesToRadians = 3.14159265f / 180.f;

/// @brief Tolerance used for floating-point comparisons throughout the renderer.
static constexpr float kEpsilon = 0.0001f;

/// @brief Pi constant for single-precision floating-point.
static constexpr float kPi = 3.14159265f;

/**
 * @brief Returns true if @p a and @p b are within @c kEpsilon of each other.
 * @param a First value.
 * @param b Second value.
 * @return true if |a - b| < kEpsilon.
 */
inline bool AreEqual(float a, float b)
{
    return std::abs(a - b) < kEpsilon;
}
