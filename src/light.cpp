#include <cmath>
#include "light.hpp"
#include "material.hpp"
#include "tuple.hpp"

bool Light::operator==(const Light &other) const
{
    return position == other.position && intensity == other.intensity;
}