#include <catch2/catch_test_macros.hpp>
#include "material.hpp"

TEST_CASE("Create a material and check default values", "[materials]")
{
    Material m = Material();

    REQUIRE(m.GetColor() == Color(1.f, 1.f, 1.f));
    REQUIRE(m.GetAmbient() == 0.1f);
    REQUIRE(m.GetDiffuse() == 0.9f);
    REQUIRE(m.GetSpecular() == 0.9f);
    REQUIRE(m.GetShininess() == 200.f);
}
