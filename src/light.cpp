#include "light.hpp"
#include "tuple.hpp"
#include <cassert>
#include <cmath>

namespace
{
bool IsFiniteColor(const Color &color)
{
    return std::isfinite(color.r) && std::isfinite(color.g) && std::isfinite(color.b);
}
}

bool Light::operator==(const Light &other) const 
{
    assert(position.IsValid());
    assert(position.IsPoint());
    assert(other.position.IsValid());
    assert(other.position.IsPoint());
    assert(IsFiniteColor(intensity));
    assert(IsFiniteColor(other.intensity));
    return position == other.position && intensity == other.intensity;
}

