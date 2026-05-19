#include "bounds.hpp"

Bounds::Bounds()
    : min(Point(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
                std::numeric_limits<float>::max())),
      max(Point(-std::numeric_limits<float>::lowest(), -std::numeric_limits<float>::lowest(),
                -std::numeric_limits<float>::lowest()))
{
}

Bounds::Bounds(const Tuple& min, const Tuple& max) : min(min), max(max)
{
}
