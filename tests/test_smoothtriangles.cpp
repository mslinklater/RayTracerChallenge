#include "shapes/smoothtriangle.hpp"
#include "tuple.hpp"
#include <catch2/catch_test_macros.hpp>

static SmoothTriangle CreateTestSmoothTriangle()
{
    Tuple p1 = Point(0.f, 1.f, 0.f);
    Tuple p2 = Point(-1.f, 0.f, 0.f);
    Tuple p3 = Point(1.f, 0.f, 0.f);
    Tuple n1 = Vector(0.f, 1.f, 0.f);
    Tuple n2 = Vector(-1.f, 0.f, 0.f);
    Tuple n3 = Vector(1.f, 0.f, 0.f);
    return SmoothTriangle(p1, p2, p3, n1, n2, n3);
}

TEST_CASE("Constructing a smooth triangle", "[smooth-triangle]")
{
    SmoothTriangle t = CreateTestSmoothTriangle();

    REQUIRE(t.GetP1() == Point(0.f, 1.f, 0.f));
    REQUIRE(t.GetP2() == Point(-1.f, 0.f, 0.f));
    REQUIRE(t.GetP3() == Point(1.f, 0.f, 0.f));
    REQUIRE(t.GetN1() == Vector(0.f, 1.f, 0.f));
    REQUIRE(t.GetN2() == Vector(-1.f, 0.f, 0.f));
    REQUIRE(t.GetN3() == Vector(1.f, 0.f, 0.f));
}
