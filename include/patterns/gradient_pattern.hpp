#pragma once
#include "pattern.hpp"

/**
 * @brief A linear gradient that blends from colour @c a to colour @c b along the X axis.
 *
 * Each unit interval in pattern space contains one full a→b transition.
 * The fractional part of @c point.x is used for interpolation.
 */
class GradientPattern : public Pattern
{
  public:
    /**
     * @brief Constructs a gradient from @p _a to @p _b.
     * @param _a Start colour (at integer X positions).
     * @param _b End colour (approached as X approaches the next integer).
     */
    GradientPattern(const Color &_a, const Color &_b) : Pattern(_a, _b)
    {
    }

    /**
     * @brief Returns the interpolated colour at @p point.
     * @param point A point in pattern space; only the X component is used.
     * @return A linearly interpolated colour between @c a and @c b.
     */
    Color PatternAt(const Tuple &point) const override
    {
        Color distance = GetB() - GetA();
        float fraction = point.x - std::floor(point.x);
        return GetA() + distance * fraction;
    }

    std::shared_ptr<Pattern> Clone() const override
    {
        return std::make_shared<GradientPattern>(*this);
    }
};
