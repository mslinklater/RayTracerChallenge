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
        // NOTE: in the book the directions here are NOT normalized, but the expected t values are based on normalized
        // directions. This is a bit inconsistent, but we'll normalize the directions here to match the expected
        // results.

        Ray r(origins[i], directions[i].Normalize());
        auto xs = cyl.IntersectLocal(r);
        REQUIRE(xs.size() == 2);
        REQUIRE(AreEqual(xs[0], expectedTs[i][0]));
        REQUIRE(AreEqual(xs[1], expectedTs[i][1]));
    }
}
