#include <catch2/catch_test_macros.hpp>
#include "sphere.hpp"
#include "intersection.hpp"
#include "ray.hpp"
#include "world.hpp"
#include "renderer.hpp"
#include "computations.hpp"

TEST_CASE("An intersection sets the object on the intersection", "[Ray]")
{
    Intersection intersection(3.5f, 42);
    REQUIRE(intersection.GetObjectId() == 42);
    REQUIRE(intersection.GetT() == 3.5f);
}

TEST_CASE("Aggregating intersections", "[Ray]")
{
    Intersection i1(1.f, 7);
    Intersection i2(2.f, 7);
    auto xs = Intersections({i1, i2});

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].GetT() == 1.f);
    REQUIRE(xs[0].GetObjectId() == 7);
    REQUIRE(xs[1].GetT() == 2.f);
    REQUIRE(xs[1].GetObjectId() == 7);
}

TEST_CASE("The hit, when all intersections have positive t", "[Ray]")
{
    Intersection i1(1.f, 1);
    Intersection i2(2.f, 1);
    auto xs = Intersections({i1, i2});
    auto hit = GetClosestIntersection(xs);

    REQUIRE(hit == i1);
}

TEST_CASE("The hit, when some intersections have negative t", "[Ray]")
{
    Intersection i1(-1.f, 1);
    Intersection i2(2.f, 1);
    auto xs = Intersections({i1, i2});
    auto hit = GetClosestIntersection(xs);

    REQUIRE(hit == i2);
}

TEST_CASE("The hit, when all intersections have negative t", "[Ray]")
{
    Intersection i1(-2.f, 1);
    Intersection i2(-1.f, 1);
    auto xs = Intersections({i1, i2});
    auto hit = GetClosestIntersection(xs);

    REQUIRE(hit.GetObjectId() == kInvalidObjectId);
}

TEST_CASE("The hit is always the lowest non-negative intersection", "[Ray]")
{
    Intersection i1(5.f, 1);
    Intersection i2(7.f, 1);
    Intersection i3(-3.f, 1);
    Intersection i4(2.f, 1);
    auto xs = Intersections({i1, i2, i3, i4});
    auto hit = GetClosestIntersection(xs);

    REQUIRE(hit == i4);
}

TEST_CASE("Precomputing the state of an intersection", "[Ray]")
{
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    World w;
    Sphere s("s");
    ObjectId id = w.AddObject(s);
    Intersection i(4.f, id);
    Computations comps = PrepareComputations(i, r, w);

    REQUIRE(comps.t == i.GetT());
    REQUIRE(comps.objectId == i.GetObjectId());
    REQUIRE(comps.point == Point(0.f, 0.f, -1.f));
    REQUIRE(comps.eyeVector == Tuple(0.f, 0.f, -1.f));
    REQUIRE(comps.normalVector == Tuple(0.f, 0.f, -1.f));
}

TEST_CASE("The hit, when an intersection occurs on the outside", "[Ray]")
{
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    World w;
    Sphere s("s");
    ObjectId id = w.AddObject(s);
    Intersection i(4.f, id);
    Computations comps = PrepareComputations(i, r, w);

    REQUIRE(comps.inside == false);
}

TEST_CASE("The hit, when an intersection occurs on the inside", "[Ray]")
{
    Ray r(Point(0.f, 0.f, 0.f), Tuple(0.f, 0.f, 1.f));
    World w;
    Sphere s("s");
    ObjectId id = w.AddObject(s);
    Intersection i(1.f, id);
    Computations comps = PrepareComputations(i, r, w);

    REQUIRE(comps.point == Point(0.f, 0.f, 1.f));
    REQUIRE(comps.eyeVector == Tuple(0.f, 0.f, -1.f));
    REQUIRE(comps.normalVector == Tuple(0.f, 0.f, -1.f));
    REQUIRE(comps.inside == true);
}