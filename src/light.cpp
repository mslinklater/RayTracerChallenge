#include "light.hpp"
#include "tuple.hpp"
#include <cassert>
#include <cmath>

bool Light::operator==(const Light &other) const
{
    assert(position.IsValid());
    assert(position.IsPoint());
    assert(other.position.IsValid());
    assert(other.position.IsPoint());
    assert(intensity.IsValid());
    assert(other.intensity.IsValid());
    return position == other.position && intensity == other.intensity;
}
