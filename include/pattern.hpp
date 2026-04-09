#pragma once
#include "color.hpp"
#include "matrix.hpp"
#include "shape.hpp"
#include "tuple.hpp"

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
    Pattern() : transform(Matrix(4)), a(kColorWhite), b(kColorBlack)
    {
        transform.SetIdentity();
    }

    /**
     * @brief Constructs a pattern with explicit colours and an identity transform.
     * @param _a Primary colour.
     * @param _b Secondary colour.
     */
    Pattern(const Color &_a, const Color &_b) : transform(Matrix(4)), a(_a), b(_b)
    {
        transform.SetIdentity();
    }

    /** @brief Sets the pattern-space transform. */
    void SetTransform(const Matrix &transform)
    {
        this->transform = transform;
    }

    /** @brief Returns the pattern-space transform. */
    Matrix GetTransform() const
    {
        return transform;
    }

    virtual ~Pattern() = default;

    /**
     * @brief Returns the pattern colour at a point in pattern space.
     * @param point A point already transformed into pattern space.
     * @return The colour at that point.
     */
    virtual Color PatternAt(const Tuple &point) = 0;

    /**
     * @brief Returns the pattern colour at a world-space @p point on @p shape.
     *
     * Converts @p point through the shape's inverse transform and then through
     * the pattern's own inverse transform before calling @c PatternAt.
     * @param shape The shape being shaded.
     * @param point The world-space point on the shape's surface.
     */
    Color PatternAtShape(const Shape &shape, const Tuple &point);

    /** @brief Returns the primary colour. */
    Color GetA() const
    {
        return a;
    }

    /** @brief Returns the secondary colour. */
    Color GetB() const
    {
        return b;
    }

  protected:
    Color a;      ///< Primary colour.
    Color b;      ///< Secondary colour.
    Matrix transform; ///< Pattern-space transform (default: identity).
};
