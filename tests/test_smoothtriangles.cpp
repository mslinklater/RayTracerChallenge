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

TEST_CASE("An intersection with a smooth triangle stores u/v", "[smooth-triangles]")
{
    SmoothTriangle t = CreateTestSmoothTriangle();

    Ray r(Point(-0.2f, 0.3f, -2.f), Vector(0.f, 0.f, 1.f));
    std::vector<Intersection> xs = t.IntersectLocal(r);
    REQUIRE(xs[0].GetU() == 0.45f);
    REQUIRE(xs[0].GetV() == 0.25f);
}

#if 0
TEST_CASE("A smooth triangle uses u/v to interpolate the normal", "[smooth-triangles]")
{
    SmoothTriangle t = CreateTestSmoothTriangle();

    Ray r(Point(-0.2f, 0.3f, -2.f), Vector(0.f, 0.f, 1.f));
    std::vector<Intersection> xs = t.IntersectLocal(r);
    Intersection hit = xs[0];
    Tuple normal = t.NormalAt(Point(0.f, 0.f, 0.f), hit);
    REQUIRE(normal == Vector(-0.5547f, 0.83205f, 0.f));
}
#endif
