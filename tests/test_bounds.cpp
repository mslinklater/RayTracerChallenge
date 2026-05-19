#include "bounds.hpp"
#include "tuple.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Create an empty bounding box", "[bounds]")
{
    Bounds b;

    REQUIRE(b.GetMin() == Point(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
                                std::numeric_limits<float>::max()));
    REQUIRE(b.GetMax() == Point(-std::numeric_limits<float>::lowest(), -std::numeric_limits<float>::lowest(),
                                -std::numeric_limits<float>::lowest()));
}
