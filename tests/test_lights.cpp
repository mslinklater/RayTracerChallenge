#include <catch2/catch_test_macros.hpp>
#include "tuple.hpp"
#include "light.hpp"
#include "color.hpp"

TEST_CASE("A point light has a position and intensity", "[light]")
{
    Color intensity = Color(1.f, 1.f, 1.f);
    Tuple position = Point(0.f, 0.f, 0.f);
    Light light(position, intensity);

    REQUIRE(light.position == position);
    REQUIRE(light.intensity == intensity);
}