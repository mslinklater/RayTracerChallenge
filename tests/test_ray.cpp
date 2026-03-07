#include <catch2/catch_test_macros.hpp>
#include "ray.hpp"
#include "tuple.hpp"
#include "intersection.hpp"

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

TEST_CASE("A ray intersects a sphere at two points", "[Ray]")
{
    Sphere sphere;
    Ray ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    auto xs = Intersect(sphere, ray);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0] == 4.f);
    REQUIRE(xs[1] == 6.f);
}

TEST_CASE("A ray intersects a sphere at tangent - two identical points", "[Ray]")
{
    Sphere sphere;
    Ray ray(Point(0.f, 1.f, -5.f), Vector(0.f, 0.f, 1.f));
    auto xs = Intersect(sphere, ray);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0] == 5.f);
    REQUIRE(xs[1] == 5.f);
}

TEST_CASE("A ray misses a sphere", "[Ray]")
{
    Sphere sphere;
    Ray ray(Point(0.f, 2.f, -5.f), Vector(0.f, 0.f, 1.f));
    auto xs = Intersect(sphere, ray);

    REQUIRE(xs.size() == 0);
}

TEST_CASE("A ray originates inside a sphere", "[Ray]")
{
    Sphere sphere;
    Ray ray(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f));
    auto xs = Intersect(sphere, ray);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0] == -1.f);
    REQUIRE(xs[1] == 1.f);
}

TEST_CASE("The sphere is behind the ray", "[Ray]")
{
    Sphere sphere;
    Ray ray(Point(0.f, 0.f, 5.f), Vector(0.f, 0.f, 1.f));
    auto xs = Intersect(sphere, ray);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0] == -6.f);
    REQUIRE(xs[1] == -4.f);
}

TEST_CASE("An intersection sets the object on the intersection", "[Ray]")
{
    Sphere sphere;
    Intersection intersection(3.5f, &sphere);
    REQUIRE(intersection.object == &sphere);
    REQUIRE(intersection.t == 3.5f);
}

TEST_CASE("Aggregating intersections", "[Ray]")
{
    Sphere sphere;
    Intersection i1(1.f, &sphere);
    Intersection i2(2.f, &sphere);
    auto xs = Intersections({i1, i2});

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].t == 1.f);
    REQUIRE(xs[0].object == &sphere);
    REQUIRE(xs[1].t == 2.f);
    REQUIRE(xs[1].object == &sphere);
}

TEST_CASE("The hit, when all intersections have positive t", "[Ray]")
{
    Sphere sphere;
    Intersection i1(1.f, &sphere);
    Intersection i2(2.f, &sphere);
    auto xs = Intersections({i1, i2});
    auto hit = Hit(xs);

    REQUIRE(hit == i1);
}

TEST_CASE("The hit, when some intersections have negative t", "[Ray]")
{
    Sphere sphere;
    Intersection i1(-1.f, &sphere);
    Intersection i2(2.f, &sphere);
    auto xs = Intersections({i1, i2});
    auto hit = Hit(xs);

    REQUIRE(hit == i2);
}

TEST_CASE("The hit, when all intersections have negative t", "[Ray]")
{
    Sphere sphere;
    Intersection i1(-2.f, &sphere);
    Intersection i2(-1.f, &sphere);
    auto xs = Intersections({i1, i2});
    auto hit = Hit(xs);

    REQUIRE(hit.object == nullptr);
}

TEST_CASE("The hit is always the lowest non-negative intersection", "[Ray]")
{
    Sphere sphere;
    Intersection i1(5.f, &sphere);
    Intersection i2(7.f, &sphere);
    Intersection i3(-3.f, &sphere);
    Intersection i4(2.f, &sphere);
    auto xs = Intersections({i1, i2, i3, i4});
    auto hit = Hit(xs);

    REQUIRE(hit == i4);
}