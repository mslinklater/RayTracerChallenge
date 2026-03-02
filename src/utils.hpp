#pragma once

#include <cmath>

bool Equal(float a, float b)
{
    return std::abs(a - b) < 0.00001f;
}
