#include "material.hpp"

bool operator==(const Material &lhs, const Material &rhs)
{
    return lhs.GetColor() == rhs.GetColor() &&
           lhs.GetAmbient() == rhs.GetAmbient() &&
           lhs.GetDiffuse() == rhs.GetDiffuse() &&
           lhs.GetSpecular() == rhs.GetSpecular() &&
           lhs.GetShininess() == rhs.GetShininess();
}
