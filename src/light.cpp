#include "light.hpp"
#include "tuple.hpp"
#include <cassert>
#include <cmath>

Light::Light(const Tuple& position, const Color& intensity) : position(position), intensity(intensity)
{
    assert(position.IsValid());
    assert(position.IsPoint());
    assert(std::isfinite(intensity.r));
    assert(std::isfinite(intensity.g));
    assert(std::isfinite(intensity.b));
}

bool Light::operator==(const Light& other) const
{
    assert(position.IsValid());
    assert(position.IsPoint());
    assert(other.position.IsValid());
    assert(other.position.IsPoint());
    assert(intensity.IsValid());
    assert(other.intensity.IsValid());
    return position == other.position && intensity == other.intensity;
}

bool Light::IsValid() const
{
    return position.IsValid() && position.IsPoint() && intensity.IsValid();
}

const Tuple& Light::GetPosition() const
{
    return position;
}

Light& Light::SetPosition(const Tuple& position)
{
    assert(position.IsValid());
    assert(position.IsPoint());
    this->position = position;
    return *this;
}

const Color& Light::GetIntensity() const
{
    return intensity;
}

Light& Light::SetIntensity(const Color& intensity)
{
    assert(intensity.IsValid());
    this->intensity = intensity;
    return *this;
}
