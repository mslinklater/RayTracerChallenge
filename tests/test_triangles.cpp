#include "triangle.hpp"
#include "tuple.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Constructing a triangle", "[triangle]")
{
    Tuple p1 = Point(0.f, 1.f, 0.f);
    Tuple p2 = Point(-1.f, 0.f, 0.f);
    Tuple p3 = Point(1.f, 0.f, 0.f);
    Triangle t(p1, p2, p3);

    REQUIRE(t.GetP1() == p1);
    REQUIRE(t.GetP2() == p2);
    REQUIRE(t.GetP3() == p3);
    REQUIRE(t.GetEdge1() == (p2 - p1));
    REQUIRE(t.GetEdge2() == (p3 - p1));
    REQUIRE(t.GetNormal() == Vector(0.f, 0.f, -1.f));
}
