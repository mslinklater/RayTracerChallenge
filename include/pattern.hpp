#pragma once
#include "color.hpp"
#include "matrix.hpp"
#include "shapes/shape.hpp"
#include "tuple.hpp"
#include <memory>

/**
 * @brief Abstract base class for 2-colour surface patterns.
 *
 * Subclasses implement @c PatternAt to define how two colours (@c a and @c b)
 * are mapped onto a surface point. The pattern can have its own transform
 * that is applied before the colour look-up.
 */
class Pattern
{
  public:
    /** @brief Constructs a default white-on-black pattern with an identity transform. */
    Pattern() : Pattern(kColorWhite, kColorBlack)
    {
    }

    /**
     * @brief Constructs a pattern with explicit colours and an identity transform.
     * @param _a Primary colour.
     * @param _b Secondary colour.
     */
    Pattern(const Color& _a, const Color& _b);

    /** @brief Sets the pattern-space transform. */
    void SetTransform(const Matrix& transform);

    /** @brief Returns the pattern-space transform. */
    const Matrix& GetTransform() const;

    virtual ~Pattern() = default;

    /**
     * @brief Returns the pattern colour at a point in pattern space.
     * @param point A point already transformed into pattern space.
     * @return The colour at that point.
     */
    virtual Color PatternAt(const Tuple& point) const = 0;

    /**
     * @brief Returns the pattern colour at a world-space @p point on @p shape.
     *
     * Converts @p point through the shape's inverse transform and then through
     * the pattern's own inverse transform before calling @c PatternAt.
     * @param shape The shape being shaded.
     * @param point The world-space point on the shape's surface.
     */
    Color PatternAtShape(const Shape& shape, const Tuple& point) const;

    /** @brief Creates a copy of this concrete pattern. */
    virtual std::shared_ptr<Pattern> Clone() const = 0;

    /** @brief Equality comparison that includes the concrete pattern type and shared base state. */
    virtual bool Equals(const Pattern& other) const;

    /** @brief Returns the primary colour. */
    Color GetA() const;

    /** @brief Returns the secondary colour. */
    Color GetB() const;

  protected:
    Color a;                 ///< Primary colour.
    Color b;                 ///< Secondary colour.
    Matrix transform;        ///< Pattern-space transform (default: identity).
    Matrix inverseTransform; ///< Cached inverse of @c transform.
};

/** @brief Equality comparison for patterns, including concrete type and transform/colour state. */
bool operator==(const Pattern& lhs, const Pattern& rhs);
