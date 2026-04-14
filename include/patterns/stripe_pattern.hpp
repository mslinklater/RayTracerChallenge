#pragma once
#include "pattern.hpp"
#include <cmath>

/**
 * @brief A pattern of alternating colour stripes along the X axis.
 *
 * Each stripe is one unit wide in pattern space. Even-indexed stripes use
 * colour @c a; odd-indexed stripes use colour @c b.
 */
class StripePattern : public Pattern
{
  public:
    /** @brief Constructs a white-on-black stripe pattern. */
    StripePattern()
    {
    }

    /**
     * @brief Constructs a stripe pattern with explicit colours.
     * @param _a Primary stripe colour.
     * @param _b Secondary stripe colour.
     */
    StripePattern(const Color &_a, const Color &_b) : Pattern(_a, _b)
    {
    }

    /**
     * @brief Returns the stripe colour at @p point based on the X coordinate.
     * @param point A point in pattern space.
     * @return @c a for even stripe indices, @c b for odd.
     */
    Color PatternAt(const Tuple &point) const override
    {
        if (static_cast<int>(std::floor(point.x)) % 2 == 0)
        {
            return GetA();
        }
        return GetB();
    }

    std::shared_ptr<Pattern> Clone() const override
    {
        return std::make_shared<StripePattern>(*this);
    }
};
