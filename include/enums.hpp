#pragma once

/**
 * @brief Indicates whether a surface point lies in shadow of a given light.
 *
 * Used as a parameter to @c Renderer::Lighting to avoid a boolean and improve
 * readability at call sites.
 */
enum class EInShadow
{
    No,  ///< The point is directly illuminated by the light.
    Yes  ///< The point is occluded by another object (in shadow).
};
