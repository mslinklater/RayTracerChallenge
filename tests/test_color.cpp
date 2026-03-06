#include <catch2/catch_test_macros.hpp>
#include "color.hpp"
#include "utils.hpp"

TEST_CASE("Color default values", "[color]")
{
    Color c;
    REQUIRE(c.r == 0.f);
    REQUIRE(c.g == 0.f);
    REQUIRE(c.b == 0.f);
}

TEST_CASE("Create color", "[color]")
{
    Color c{-0.5f, 0.4f, 1.7f};
    REQUIRE(c.r == -0.5f);
    REQUIRE(c.g == 0.4f);
    REQUIRE(c.b == 1.7f);
}

TEST_CASE("Adding colors", "[color]")
{
    Color c1{0.9f, 0.6f, 0.75f};
    Color c2{0.7f, 0.1f, 0.25f};
    Color result = c1 + c2;
    REQUIRE(AreEqual(result.r, 1.6f));
    REQUIRE(AreEqual(result.g, 0.7f));
    REQUIRE(AreEqual(result.b, 1.0f));
}

TEST_CASE("Subtracting colors", "[color]")
{
    Color c1{0.9f, 0.6f, 0.75f};
    Color c2{0.7f, 0.1f, 0.25f};
    Color result = c1 - c2;
    REQUIRE(AreEqual(result.r, 0.2f));
    REQUIRE(AreEqual(result.g, 0.5f));
    REQUIRE(AreEqual(result.b, 0.5f));
}

TEST_CASE("Multiplying a color by a scalar", "[color]")
{
    Color c{0.2f, 0.3f, 0.4f};
    Color result = c * 2.f;
    REQUIRE(AreEqual(result.r, 0.4f));
    REQUIRE(AreEqual(result.g, 0.6f));
    REQUIRE(AreEqual(result.b, 0.8f));
}

TEST_CASE("Multiplying colors", "[color]")
{
    Color c1{1.f, 0.2f, 0.4f};
    Color c2{0.9f, 1.f, 0.1f};
    Color result = c1 * c2;
    REQUIRE(AreEqual(result.r, 0.9f));
    REQUIRE(AreEqual(result.g, 0.2f));
    REQUIRE(AreEqual(result.b, 0.04f));
}