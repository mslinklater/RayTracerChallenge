#pragma once
#include "tuple.hpp"

struct Matrix;

/**
 * @brief An infinite ray defined by an origin point and a direction vector.
 *
 * Rays are the fundamental primitive used to query the scene during rendering.
 * They can be transformed into object space via @c operator*(Ray, Matrix).
 */
class Ray {
public:
    /** @brief Default constructor: ray at the origin pointing along +Z. */
    Ray() : origin(0.f, 0.f, 0.f, 1.f), direction(0.f, 0.f, 1.f, 0.f) {}

    /**
     * @brief Constructs a ray with explicit origin and direction.
     * @param origin    The starting point of the ray (point tuple, w=1).
     * @param direction The direction of the ray (vector tuple, w=0).
     */
    Ray(const Tuple &origin, const Tuple &direction)
        : origin(origin), direction(direction) {}

    /** @brief Returns the origin point of the ray. */
    Tuple GetOrigin() const { return origin; }

    /** @brief Returns the direction vector of the ray. */
    Tuple GetDirection() const { return direction; }

    /**
     * @brief Returns the position along the ray at parameter @p t.
     * @param t The ray parameter (distance along the direction vector).
     * @return The point @c origin + @c direction * t.
     */
    Tuple PositionAt(float t) const { return origin + direction * t; }

private:
    Tuple origin;    ///< The starting point of the ray.
    Tuple direction; ///< The direction of the ray.
};

/**
 * @brief Transforms a ray by a matrix, producing a ray in a new space.
 * @param ray    The ray to transform.
 * @param matrix The transformation matrix to apply.
 * @return A new ray with both origin and direction transformed.
 */
extern Ray operator*(const Ray &ray, const Matrix &matrix);
