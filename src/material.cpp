#include "material.hpp"
#include "maths.hpp"

bool operator==(const Material &lhs, const Material &rhs)
{
    return lhs.GetColor() == rhs.GetColor() && AreEqual(lhs.GetAmbient(), rhs.GetAmbient()) &&
           AreEqual(lhs.GetDiffuse(), rhs.GetDiffuse()) && AreEqual(lhs.GetSpecular(), rhs.GetSpecular()) &&
           AreEqual(lhs.GetShininess(), rhs.GetShininess());
}
