#pragma once

struct Color
{
    float r = 0.f;
    float g = 0.f;
    float b = 0.f;

    Color operator+(const Color &other) const;
    Color operator-(const Color &other) const;
    Color operator*(float scalar) const;
    Color operator*(const Color &other) const;
    bool operator==(const Color &other) const;
};

static constexpr Color kBackgroundColor(0.f, 1.f, 1.f);