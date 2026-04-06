#pragma once
#include <iostream>

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

    void Cout() const
    {
        std::cout << "Color(" << r << ", " << g << ", " << b << ")" << std::endl;
    }
};

// Color to represent a ray that never hits anything
static constexpr Color kBackgroundColor(0.f, 1.f, 1.f);

// color constants
static constexpr Color kColorBlack(0.f, 0.f, 0.f);
static constexpr Color kColorRed(1.f, 0.f, 0.f);
static constexpr Color kColorGreen(0.f, 1.f, 0.f);
static constexpr Color kColorBlue(0.f, 0.f, 1.f);
static constexpr Color kColorYellow(1.f, 1.f, 0.f);
static constexpr Color kColorMagenta(1.f, 0.f, 1.f);
static constexpr Color kColorCyan(0.f, 1.f, 1.f);
static constexpr Color kColorWhite(1.f, 1.f, 1.f);
