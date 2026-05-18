#include <catch2/catch_test_macros.hpp>

TEST_CASE("Create an empty bounding box", "[bounds]")
{
    Bounds b;
    REQUIRE(b.GetMin() == Point(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(),
                                std::numeric_limits<float>::infinity()));
    REQUIRE(b.GetMax() == Point(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(),
                                -std::numeric_limits<float>::infinity()));
}
