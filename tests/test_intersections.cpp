#include "computations.hpp"
#include "intersection.hpp"
#include "maths.hpp"
#include "plane.hpp"
#include "ray.hpp"
#include "renderer.hpp"
#include "sphere.hpp"
#include "utils.hpp"
#include "world.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("An intersection sets the object on the intersection", "[ray]")
{
    Intersection intersection(3.5f, 42);
    REQUIRE(intersection.GetObjectId() == 42);
    REQUIRE(intersection.GetT() == 3.5f);
}

TEST_CASE("Aggregating intersections", "[Ray]")
{
    Intersection i1(1.f, 7);
    Intersection i2(2.f, 7);
    auto xs = Renderer::Intersections({i1, i2});

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].GetT() == 1.f);
    REQUIRE(xs[0].GetObjectId() == 7);
    REQUIRE(xs[1].GetT() == 2.f);
    REQUIRE(xs[1].GetObjectId() == 7);
}

TEST_CASE("The hit, when all intersections have positive t", "[ray]")
{
    Intersection i1(1.f, 1);
    Intersection i2(2.f, 1);
    auto xs = Renderer::Intersections({i1, i2});
    auto hit = Renderer::GetClosestIntersection(xs);

    REQUIRE(hit == i1);
}

TEST_CASE("The hit, when some intersections have negative t", "[ray]")
{
    Intersection i1(-1.f, 1);
    Intersection i2(2.f, 1);
    auto xs = Renderer::Intersections({i1, i2});
    auto hit = Renderer::GetClosestIntersection(xs);

    REQUIRE(hit == i2);
}

TEST_CASE("The hit, when all intersections have negative t", "[ray]")
{
    Intersection i1(-2.f, 1);
    Intersection i2(-1.f, 1);
    auto xs = Renderer::Intersections({i1, i2});
    auto hit = Renderer::GetClosestIntersection(xs);

    REQUIRE(hit.GetObjectId() == kInvalidObjectId);
}

TEST_CASE("The hit is always the lowest non-negative intersection", "[ray]")
{
    Intersection i1(5.f, 1);
    Intersection i2(7.f, 1);
    Intersection i3(-3.f, 1);
    Intersection i4(2.f, 1);
    auto xs = Renderer::Intersections({i1, i2, i3, i4});
    auto hit = Renderer::GetClosestIntersection(xs);

    REQUIRE(hit == i4);
}

TEST_CASE("Precomputing the state of an intersection", "[ray]")
{
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    World w;
    Sphere s("s");
    ObjectId id = w.AddObject(s);
    Intersection i(4.f, id);
    Computations comps = Renderer::PrepareComputations(i, r, w);

    REQUIRE(comps.t == i.GetT());
    REQUIRE(comps.objectId == i.GetObjectId());
    REQUIRE(comps.point == Point(0.f, 0.f, -1.f));
    REQUIRE(comps.eyeVector == Tuple(0.f, 0.f, -1.f));
    REQUIRE(comps.normalVector == Tuple(0.f, 0.f, -1.f));
}

TEST_CASE("The hit, when an intersection occurs on the outside", "[ray]")
{
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    World w;
    Sphere s("s");
    ObjectId id = w.AddObject(s);
    Intersection i(4.f, id);
    Computations comps = Renderer::PrepareComputations(i, r, w);

    REQUIRE(comps.inside == false);
}

TEST_CASE("The hit, when an intersection occurs on the inside", "[ray]")
{
    Ray r(Point(0.f, 0.f, 0.f), Tuple(0.f, 0.f, 1.f));
    World w;
    Sphere s("s");
    ObjectId id = w.AddObject(s);
    Intersection i(1.f, id);
    Computations comps = Renderer::PrepareComputations(i, r, w);

    REQUIRE(comps.point == Point(0.f, 0.f, 1.f));
    REQUIRE(comps.eyeVector == Tuple(0.f, 0.f, -1.f));
    REQUIRE(comps.normalVector == Tuple(0.f, 0.f, -1.f));
    REQUIRE(comps.inside == true);
}

TEST_CASE("The hit should offset the point", "[ray]")
{
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    World w;
    Sphere s("s");
    s.SetTransform(Matrix::CreateTranslation(0.f, 0.f, 1.f));
    ObjectId id = w.AddObject(s);
    Intersection i(5.f, id);
    Computations comps = Renderer::PrepareComputations(i, r, w);

    REQUIRE(comps.overPoint.z < kEpsilon / 2.f);
    REQUIRE(comps.point.z > comps.overPoint.z);
}

TEST_CASE("Precomputing the reflection vector", "[ray]")
{
    World w;
    Plane shape("plane");
    w.AddObject(shape);
    Ray r(Point(0.f, 1.f, -1.f), Tuple(0.f, -std::sqrt(2.f) / 2.f, std::sqrt(2.f) / 2.f));
    Intersection i(std::sqrt(2.f), shape.GetWorldObjectId());
    Computations comps = Renderer::PrepareComputations(i, r, w);
    REQUIRE(comps.reflectv == Vector(0.f, std::sqrt(2.f) / 2.f, std::sqrt(2.f) / 2.f));
}

TEST_CASE("Finding n1 and n2 at various intersections")
{
    World w;
    Sphere a = GlassSphere("a");
    a.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    a.GetMutableMaterial().SetRefractiveIndex(1.5f);
    w.AddObject(a);

    Sphere b = GlassSphere("b");
    b.SetTransform(Matrix::CreateTranslation(0.f, 0.f, -.25f));
    b.GetMutableMaterial().SetRefractiveIndex(2.0f);
    w.AddObject(b);

    Sphere c = GlassSphere("c");
    c.SetTransform(Matrix::CreateTranslation(0.f, 0.f, 0.25f));
    c.GetMutableMaterial().SetRefractiveIndex(2.5f);
    w.AddObject(c);

    Ray r(Point(0.f, 0.f, -4.f), Tuple(0.f, 0.f, 1.f));
    IntersectionVector xs = {
        Intersection(2.f, a.GetWorldObjectId()),   Intersection(2.75f, b.GetWorldObjectId()),
        Intersection(3.25f, c.GetWorldObjectId()), Intersection(4.75f, b.GetWorldObjectId()),
        Intersection(5.25f, c.GetWorldObjectId()), Intersection(6.f, a.GetWorldObjectId()),
    };

    Computations comps0 = Renderer::PrepareComputations(xs[0], r, w, &xs);
    REQUIRE(comps0.n1 == 1.f);
    REQUIRE(comps0.n2 == 1.5f);

    Computations comps1 = Renderer::PrepareComputations(xs[1], r, w, &xs);
    REQUIRE(comps1.n1 == 1.5f);
    REQUIRE(comps1.n2 == 2.0f);

    Computations comps2 = Renderer::PrepareComputations(xs[2], r, w, &xs);
    REQUIRE(comps2.n1 == 2.0f);
    REQUIRE(comps2.n2 == 2.5f);

    Computations comps3 = Renderer::PrepareComputations(xs[3], r, w, &xs);
    REQUIRE(comps3.n1 == 2.5f);
    REQUIRE(comps3.n2 == 2.5f);

    Computations comps4 = Renderer::PrepareComputations(xs[4], r, w, &xs);
    REQUIRE(comps4.n1 == 2.5f);
    REQUIRE(comps4.n2 == 1.5f);

    Computations comps5 = Renderer::PrepareComputations(xs[5], r, w, &xs);
    REQUIRE(comps5.n1 == 1.5f);
    REQUIRE(comps5.n2 == 1.0f);
}

TEST_CASE("The under point is offset below the surface", "[ray]")
{
    World w;
    Sphere s = GlassSphere("s");
    s.SetTransform(Matrix::CreateTranslation(0.f, 0.f, 1.f));
    w.AddObject(s);

    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    Intersection i(5.f, s.GetWorldObjectId());
    IntersectionVector xs = {i};
    Computations comps = Renderer::PrepareComputations(i, r, w, &xs);

    REQUIRE(comps.underPoint.z > kEpsilon / 2.f);
    REQUIRE(comps.point.z < comps.underPoint.z);
}
