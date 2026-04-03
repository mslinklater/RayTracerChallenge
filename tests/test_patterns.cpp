#include "color.hpp"
#include "pattern.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Creating a stripe pattern", "[patterns]")
{
    StripePattern pattern(kColorWhite, kColorBlack);
    REQUIRE(pattern.GetA() == kColorWhite);
    REQUIRE(pattern.GetB() == kColorBlack);
}

TEST_CASE("A stripe pattern is constant in y", "[patterns]")
{
    StripePattern pattern(kColorWhite, kColorBlack);
    REQUIRE(pattern.StripeAt(Point(0.f, 0.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.StripeAt(Point(0.f, 1.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.StripeAt(Point(0.f, 2.f, 0.f)) == kColorWhite);
}

TEST_CASE("A stripe pattern is constant in z", "[patterns]")
{
    StripePattern pattern(kColorWhite, kColorBlack);
    REQUIRE(pattern.StripeAt(Point(0.f, 0.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.StripeAt(Point(0.f, 0.f, 1.f)) == kColorWhite);
    REQUIRE(pattern.StripeAt(Point(0.f, 0.f, 2.f)) == kColorWhite);
}

TEST_CASE("A stripe pattern alternates in x", "[patterns]")
{
    StripePattern pattern(kColorWhite, kColorBlack);
    REQUIRE(pattern.StripeAt(Point(0.f, 0.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.StripeAt(Point(0.9f, 0.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.StripeAt(Point(1.f, 0.f, 0.f)) == kColorBlack);
    REQUIRE(pattern.StripeAt(Point(-0.1f, 0.f, 0.f)) == kColorBlack);
    REQUIRE(pattern.StripeAt(Point(-1.f, 0.f, 0.f)) == kColorBlack);
    REQUIRE(pattern.StripeAt(Point(-1.1f, 0.f, 0.f)) == kColorWhite);
}
