#include <cmath>

static constexpr float kDegreesToRadians = 3.14159265f / 180.f;

inline bool AreEqual(float a, float b)
{
    return std::abs(a - b) < 0.0001f;
}
