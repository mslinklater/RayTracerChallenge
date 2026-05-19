#include "boundingbox.hpp"
#include "shapes/sphere.hpp"
#include "tuple.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Create an empty bounding box", "[bounds]")
{
    BoundingBox b;

    REQUIRE(b.GetMin() == Point(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
                                std::numeric_limits<float>::max()));
    REQUIRE(b.GetMax() == Point(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(),
                                std::numeric_limits<float>::lowest()));
}

TEST_CASE("Creating abounding box with a volume", "[bounds]")
{
    BoundingBox b(Point(-1.f, -2.f, -3.f), Point(3.f, 2.f, 1.f));
    REQUIRE(b.GetMin() == Point(-1.f, -2.f, -3.f));
    REQUIRE(b.GetMax() == Point(3.f, 2.f, 1.f));
}

TEST_CASE("Adding points to an empty bounding box", "[bounds]")
{
    BoundingBox b;
    b.AddPoint(Point(-5.f, 2.f, 0.f));
    b.AddPoint(Point(7.f, 0.f, -3.f));
    REQUIRE(b.GetMin() == Point(-5.f, 0.f, -3.f));
    REQUIRE(b.GetMax() == Point(7.f, 2.f, 0.f));
}

TEST_CASE("Adding points to a non-empty bounding box", "[bounds]")
{
    BoundingBox b(Point(-5.f, 2.f, 0.f), Point(7.f, 0.f, -3.f));
    b.AddPoint(Point(-6.f, 3.f, 1.f));
    b.AddPoint(Point(8.f, -1.f, -4.f));
    REQUIRE(b.GetMin() == Point(-6.f, -1.f, -4.f));
    REQUIRE(b.GetMax() == Point(8.f, 3.f, 1.f));
}

TEST_CASE("Adding points that don't change the bounding box", "[bounds]")
{
    BoundingBox b(Point(-5.f, 2.f, 0.f), Point(7.f, 0.f, -3.f));
    b.AddPoint(Point(-4.f, 1.f, -1.f));
    b.AddPoint(Point(6.f, 0.5f, -2.f));

    REQUIRE(b.GetMin() == Point(-5.f, 0.f, -3.f));
    REQUIRE(b.GetMax() == Point(7.f, 2.f, 0.f));
}

TEST_CASE("A sphere has a bounding box", "[bounds]")
{
    Sphere s("sphere");
    BoundingBox b = s.GetBounds();
    REQUIRE(b.GetMin() == Point(-1.f, -1.f, -1.f));
    REQUIRE(b.GetMax() == Point(1.f, 1.f, 1.f));
}

TEST_CASE("A plane has a bounding box", "[bounds]")
{
}
