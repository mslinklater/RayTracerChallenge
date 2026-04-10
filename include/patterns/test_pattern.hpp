#include "pattern.hpp"

/**
 * @brief A test pattern that returns a color based on the input tuple
 */
class TestPattern : public Pattern
{
  public:
    Color PatternAt(const Tuple &point) override
    {
        return Color(point.x, point.y, point.z);
    }
};
