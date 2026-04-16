#pragma once
#include <cassert>
#include <cmath>

/**
 * @brief A 4D homogeneous coordinate used to represent both points and vectors.
 *
 * When @c w == 1, the tuple is a point in 3D space.
 * When @c w == 0, the tuple is a direction vector.
 */
struct Tuple
{
    float x; ///< X component.
    float y; ///< Y component.
    float z; ///< Z component.
    float w; ///< Homogeneous component: 1 for points, 0 for vectors.

    /** @brief Adds two tuples component-wise. */
    Tuple operator+(const Tuple &other) const;
    /** @brief Subtracts @p other from this tuple component-wise. */
    Tuple operator-(const Tuple &other) const;
    /** @brief Negates all components. */
    Tuple operator-() const;
    /** @brief Scales all components by @p scalar. */
    Tuple operator*(float scalar) const;
    /** @brief Divides all components by @p scalar. */
    Tuple operator/(float scalar) const;

    /**
     * @brief Computes the dot product with @p other (operator|).
     * @return The scalar dot product.
     */
    float operator|(const Tuple &other) const;

    /**
     * @brief Computes the cross product with @p other (operator^).
     * @return A new vector perpendicular to both operands.
     */
    Tuple operator^(const Tuple &other) const;

    /** @brief Returns the Euclidean length of this tuple. */
    float Magnitude() const;
    /** @brief Returns true if this tuple represents a point (w == 1). */
    bool IsPoint() const;
    /** @brief Returns true if this tuple represents a vector (w == 0). */
    bool IsVector() const;
    /** @brief Returns true when all components are finite values. */
    bool IsValid() const;
    /**
     * @brief Compares two tuples using floating-point tolerance.
     * @return true if all components are within epsilon of each other.
     */
    bool IsEqual(const Tuple &other) const;
    /** @brief Equality operator using floating-point tolerance. */
    bool operator==(const Tuple &other) const;

    /** @brief Returns a unit-length copy of this tuple. */
    Tuple Normalize() const;

    /**
     * @brief Reflects this tuple about @p normal.
     * @param normal The surface normal to reflect around (must be normalised).
     * @return The reflected tuple.
     */
    Tuple Reflect(const Tuple &normal) const;
};

/**
 * @brief Constructs a point tuple (w = 1).
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param z Z coordinate.
 */
inline Tuple Point(float x, float y, float z)
{
    assert(std::isfinite(x));
    assert(std::isfinite(y));
    assert(std::isfinite(z));
    return Tuple{x, y, z, 1.f};
}

/**
 * @brief Constructs a direction vector tuple (w = 0).
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 */
inline Tuple Vector(float x, float y, float z)
{
    return Tuple{x, y, z, 0.f};
}