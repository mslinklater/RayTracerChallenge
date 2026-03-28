#include <catch2/catch_test_macros.hpp>
#include "tuple.hpp"
#include "plane.hpp"

TEST_CASE("The normal of a plane is constant everywhere", "[Planes]")
{
    Plane plane("plane");
    Tuple n1 = plane.NormalAt(Point(0.f, 0.f, 0.f));
    Tuple n2 = plane.NormalAt(Point(10.f, 0.f, -10.f));
    Tuple n3 = plane.NormalAt(Point(-5.f, 0.f, 150.f));

    REQUIRE(n1 == Vector(0.f, 1.f, 0.f));
    REQUIRE(n2 == Vector(0.f, 1.f, 0.f));
    REQUIRE(n3 == Vector(0.f, 1.f, 0.f));
}

TEST_CASE("Intersect with a ray parallel to the plane", "[Planes]")
{
    Plane plane("plane");
    Ray ray(Point(0.f, 10.f, 0.f), Vector(0.f, 0.f, 1.f));
    auto xs = plane.IntersectLocal(ray);

    REQUIRE(xs.size() == 0);
}

TEST_CASE("Intersect with a coplanar ray", "[Planes]")
{
    Plane plane("plane");
    Ray ray(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f));
    auto xs = plane.IntersectLocal(ray);

    REQUIRE(xs.size() == 0);
}