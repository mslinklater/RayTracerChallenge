#include "maths.hpp"
#include "ray.hpp"
#include "shapes/cylinder.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("A ray misses a cylinder", "[cylinders]")
{
    Cylinder cyl("cylinder");
    std::vector<Tuple> origins = {Point(1.0f, 0.0f, 0.0f), Point(0.0f, 1.0f, 0.0f), Point(0.0f, 0.0f, -5.0f)};
    std::vector<Tuple> directions = {Vector(0.0f, 1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f), Vector(1.0f, 1.0f, 1.0f)};

    for (size_t i = 0; i < origins.size(); ++i)
    {
        Ray r(origins[i], directions[i]);
        auto xs = cyl.IntersectLocal(r);
        REQUIRE(xs.empty());
    }
}

TEST_CASE("A ray strikes a cylinder", "[cylinders]")
{
    Cylinder cyl("cylinder");
    std::vector<Tuple> origins = {Point(1.0f, 0.0f, -5.0f), Point(0.0f, 0.0f, -5.0f), Point(0.5f, 0.0f, -5.0f)};
    std::vector<Tuple> directions = {Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, 0.0f, 1.0f), Vector(0.1f, 1.0f, 1.0f)};
    std::vector<std::vector<float>> expectedTs = {{5.0f, 5.0f}, {4.0f, 6.0f}, {6.80798f, 7.08872f}};

    for (size_t i = 0; i < origins.size(); ++i)
    {
        Ray r(origins[i], directions[i].Normalize());
        auto xs = cyl.IntersectLocal(r);
        REQUIRE(xs.size() == 2);
        REQUIRE(AreEqual(xs[0], expectedTs[i][0]));
        REQUIRE(AreEqual(xs[1], expectedTs[i][1]));
    }
}

TEST_CASE("Normal vector on a cylinder", "[cylinders]")
{
    Cylinder cyl("cylinder");
    std::vector<Tuple> points = {Point(1.0f, 0.0f, 0.0f), Point(0.0f, 5.0f, -1.0f), Point(0.0f, -2.0f, 1.0f),
                                 Point(-1.0f, 1.0f, 0.0f)};
    std::vector<Tuple> expectedNormals = {Vector(1.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, -1.0f), Vector(0.0f, 0.0f, 1.0f),
                                          Vector(-1.0f, 0.0f, 0.0f)};

    for (size_t i = 0; i < points.size(); ++i)
    {
        Tuple n = cyl.NormalAtLocal(points[i]);
        REQUIRE(n == expectedNormals[i]);
    }
}

TEST_CASE("The default minimum and maximum for a cylinder", "[cylinders]")
{
    Cylinder cyl("cylinder");
    REQUIRE(cyl.GetMinimum() == -std::numeric_limits<float>::infinity());
    REQUIRE(cyl.GetMaximum() == std::numeric_limits<float>::infinity());
}

TEST_CASE("Intersecting a constrained cylinder", "[cylinders]")
{
    Cylinder cyl("cylinder");
    cyl.SetMinimum(1.0f);
    cyl.SetMaximum(2.0f);
    std::vector<Tuple> origins = {Point(0.0f, 1.5f, 0.0f),  Point(0.0f, 3.0f, -5.0f), Point(0.0f, 0.0f, -5.0f),
                                  Point(0.0f, 2.5f, -5.0f), Point(0.0f, 1.0f, -5.0f), Point(0.0f, 1.5f, -2.0f)};
    std::vector<Tuple> directions = {Vector(0.1f, 1.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, 0.0f, 1.0f),
                                     Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, 0.0f, 1.0f)};
    std::vector<int> expectedCounts = {0, 0, 0, 0, 0, 2};

    for (size_t i = 0; i < origins.size(); ++i)
    {
        Ray r(origins[i], directions[i].Normalize());
        auto xs = cyl.IntersectLocal(r);
        REQUIRE(xs.size() == expectedCounts[i]);
    }
}
