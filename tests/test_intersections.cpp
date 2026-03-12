#include <catch2/catch_test_macros.hpp>
#include "sphere.hpp"
#include "intersection.hpp"

TEST_CASE("An intersection sets the object on the intersection", "[Ray]")
{
    Sphere sphere;
    Intersection intersection(3.5f, &sphere);
    REQUIRE(intersection.GetObject() == &sphere);
    REQUIRE(intersection.GetT() == 3.5f);
}

TEST_CASE("Aggregating intersections", "[Ray]")
{
    Sphere sphere;
    Intersection i1(1.f, &sphere);
    Intersection i2(2.f, &sphere);
    auto xs = Intersections({i1, i2});

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].GetT() == 1.f);
    REQUIRE(xs[0].GetObject() == &sphere);
    REQUIRE(xs[1].GetT() == 2.f);
    REQUIRE(xs[1].GetObject() == &sphere);
}

TEST_CASE("The hit, when all intersections have positive t", "[Ray]")
{
    Sphere sphere;
    Intersection i1(1.f, &sphere);
    Intersection i2(2.f, &sphere);
    auto xs = Intersections({i1, i2});
    auto hit = GetClosestIntersection(xs);

    REQUIRE(hit == i1);
}

TEST_CASE("The hit, when some intersections have negative t", "[Ray]")
{
    Sphere sphere;
    Intersection i1(-1.f, &sphere);
    Intersection i2(2.f, &sphere);
    auto xs = Intersections({i1, i2});
    auto hit = GetClosestIntersection(xs);

    REQUIRE(hit == i2);
}

TEST_CASE("The hit, when all intersections have negative t", "[Ray]")
{
    Sphere sphere;
    Intersection i1(-2.f, &sphere);
    Intersection i2(-1.f, &sphere);
    auto xs = Intersections({i1, i2});
    auto hit = GetClosestIntersection(xs);

    REQUIRE(hit.GetObject() == nullptr);
}

TEST_CASE("The hit is always the lowest non-negative intersection", "[Ray]")
{
    Sphere sphere;
    Intersection i1(5.f, &sphere);
    Intersection i2(7.f, &sphere);
    Intersection i3(-3.f, &sphere);
    Intersection i4(2.f, &sphere);
    auto xs = Intersections({i1, i2, i3, i4});
    auto hit = GetClosestIntersection(xs);

    REQUIRE(hit == i4);
}