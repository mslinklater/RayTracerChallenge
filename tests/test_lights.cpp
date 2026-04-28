#include "color.hpp"
#include "light.hpp"
#include "tuple.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("A point light has a position and intensity", "[light]")
{
    Color intensity = Color(1.f, 1.f, 1.f);
    Tuple position = Point(0.f, 0.f, 0.f);
    Light light(position, intensity);

    REQUIRE(light.GetPosition() == position);
    REQUIRE(light.GetIntensity() == intensity);
}
