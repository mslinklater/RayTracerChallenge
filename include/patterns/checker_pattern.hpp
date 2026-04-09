#pragma once
#include "pattern.hpp"

/**
 * @brief A 3D checkerboard pattern that alternates colours in all three axes.
 *
 * The check size is one unit in pattern space. Even-parity cells (where the sum
 * of floored coordinates is even) use colour @c a; odd-parity cells use @c b.
 */
class CheckerPattern : public Pattern
{
  public:
    /** @brief Constructs a white-on-black checkerboard pattern. */
    CheckerPattern()
    {
    }

    /**
     * @brief Constructs a checkerboard with explicit colours.
     * @param _a Primary cell colour (even-parity cells).
     * @param _b Secondary cell colour (odd-parity cells).
     */
    CheckerPattern(const Color &_a, const Color &_b) : Pattern(_a, _b)
    {
    }

    /**
     * @brief Returns the checkerboard colour at @p point.
     * @param point A point in pattern space.
     * @return @c a when floor(x)+floor(y)+floor(z) is even, @c b otherwise.
     */
    Color PatternAt(const Tuple &point) override
    {
        if (static_cast<int>(std::floor(point.x) + std::floor(point.y) + std::floor(point.z)) % 2 == 0)
        {
            return GetA();
        }
        return GetB();
    }
};
