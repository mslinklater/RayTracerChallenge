#pragma once

#include <cmath>

bool AreEqual(float a, float b)
{
    return std::abs(a - b) < 0.00001f;
}
