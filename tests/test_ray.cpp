#include <catch2/catch_test_macros.hpp>
#include "ray.hpp"
#include "tuple.hpp"

TEST_CASE("A ray is created with an origin and a direction", "[Ray]")
{
    Tuple origin = Point(1.f, 2.f, 3.f);
    Tuple direction = Vector(4.f, 5.f, 6.f);
    Ray ray(origin, direction);

    REQUIRE(ray.origin == origin);
    REQUIRE(ray.direction == direction);
}

TEST_CASE("Computing a point from a distance", "[Ray]")
{
    Ray ray(Point(2.f, 3.f, 4.f), Vector(1.f, 0.f, 0.f));

    REQUIRE(Position(ray, 0.f) == Point(2.f, 3.f, 4.f));
    REQUIRE(Position(ray, 1.f) == Point(3.f, 3.f, 4.f));
    REQUIRE(Position(ray, -1.f) == Point(1.f, 3.f, 4.f));
    REQUIRE(Position(ray, 2.5f) == Point(4.5f, 3.f, 4.f));
}