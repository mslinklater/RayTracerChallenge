#include "shapes/triangle.hpp"
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

TEST_CASE("Finding the normal on a triangle", "[triangle]")
{
    Triangle t(Point(0.f, 1.f, 0.f), Point(-1.f, 0.f, 0.f), Point(1.f, 0.f, 0.f));
    Intersection i;
    Tuple n1 = t.NormalAt(Point(0.f, 0.5f, 0.f), i);
    Tuple n2 = t.NormalAt(Point(-0.5f, 0.75f, 0.f), i);
    Tuple n3 = t.NormalAt(Point(0.5f, 0.25f, 0.f), i);
    REQUIRE(n1 == t.GetNormal());
    REQUIRE(n2 == t.GetNormal());
    REQUIRE(n3 == t.GetNormal());
}

TEST_CASE("Intersecting a ray parallel to the triangle", "[triangle]")
{
    Triangle t(Point(0.f, 1.f, 0.f), Point(-1.f, 0.f, 0.f), Point(1.f, 0.f, 0.f));
    Ray r(Point(0.f, -1.f, -2.f), Vector(0.f, 1.f, 0.f));
    std::vector<Intersection> xs = t.IntersectLocal(r);
    REQUIRE(xs.empty());
}

TEST_CASE("A ray misses the p1-p3 edge", "[triangle]")
{
    Triangle t(Point(0.f, 1.f, 0.f), Point(-1.f, 0.f, 0.f), Point(1.f, 0.f, 0.f));
    Ray r(Point(1.f, 1.f, -2.f), Vector(0.f, 0.f, 1.f));
    std::vector<Intersection> xs = t.IntersectLocal(r);
    REQUIRE(xs.empty());
}

TEST_CASE("A ray misses the p1-p2 edge", "[triangle]")
{
    Triangle t(Point(0.f, 1.f, 0.f), Point(-1.f, 0.f, 0.f), Point(1.f, 0.f, 0.f));
    Ray r(Point(-1.f, 1.f, -2.f), Vector(0.f, 0.f, 1.f));
    std::vector<Intersection> xs = t.IntersectLocal(r);
    REQUIRE(xs.empty());
}

TEST_CASE("A ray misses the p2-p3 edge", "[triangle]")
{
    Triangle t(Point(0.f, 1.f, 0.f), Point(-1.f, 0.f, 0.f), Point(1.f, 0.f, 0.f));
    Ray r(Point(0.f, -1.f, -2.f), Vector(0.f, 0.f, 1.f));
    std::vector<Intersection> xs = t.IntersectLocal(r);
    REQUIRE(xs.empty());
}

TEST_CASE("A ray strikes a triangle", "[triangle]")
{
    Triangle t(Point(0.f, 1.f, 0.f), Point(-1.f, 0.f, 0.f), Point(1.f, 0.f, 0.f));
    Ray r(Point(0.f, 0.5f, -2.f), Vector(0.f, 0.f, 1.f));
    std::vector<Intersection> xs = t.IntersectLocal(r);
    REQUIRE(xs.size() == 1);
    REQUIRE(xs[0].GetT() == 2.f);
}
