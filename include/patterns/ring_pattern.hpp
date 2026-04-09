#pragma once
#include "pattern.hpp"

/**
 * @brief A concentric ring pattern based on the distance from the Y axis.
 *
 * Rings extend in the XZ plane. Each ring is one unit wide. Even-indexed rings
 * use colour @c a; odd-indexed rings use colour @c b.
 */
class RingPattern : public Pattern
{
  public:
    /** @brief Constructs a white-on-black ring pattern. */
    RingPattern()
    {
    }

    /**
     * @brief Constructs a ring pattern with explicit colours.
     * @param _a Primary ring colour.
     * @param _b Secondary ring colour.
     */
    RingPattern(const Color &_a, const Color &_b) : Pattern(_a, _b)
    {
    }

    /**
     * @brief Returns the ring colour at @p point based on its XZ distance from the origin.
     * @param point A point in pattern space.
     * @return @c a for even ring indices, @c b for odd.
     */
    Color PatternAt(const Tuple &point) override
    {
        if (static_cast<int>(std::floor(sqrt(point.x * point.x + point.z * point.z))) % 2 == 0)
        {
            return GetA();
        }
        return GetB();
    }
};
