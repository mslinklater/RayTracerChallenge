#include "ray.hpp"
#include "shapes/cube.hpp"
#include "tuple.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("A ray intersects a cube", "[cubes]")
{
    Cube cube("cube");

    Tuple points[] = {Point(5.f, 0.5f, 0.f), Point(-5.f, 0.5f, 0.f), Point(0.5f, 5.f, 0.f), Point(0.5f, -5.f, 0.f),
                      Point(0.5f, 0.f, 5.f), Point(0.5f, 0.f, -5.f), Point(0.f, 0.5f, 0.f)};
    Tuple directions[] = {Vector(-1.f, 0.f, 0.f), Vector(1.f, 0.f, 0.f), Vector(0.f, -1.f, 0.f), Vector(0.f, 1.f, 0.f),
                          Vector(0.f, 0.f, -1.f), Vector(0.f, 0.f, 1.f), Vector(0.f, 0.f, 1.f)};
    float t1[] = {4.f, 4.f, 4.f, 4.f, 4.f, 4.f, -1.f};
    float t2[] = {6.f, 6.f, 6.f, 6.f, 6.f, 6.f, 1.f};

    for (int i = 0; i < 7; ++i)
    {
        Ray ray(points[i], directions[i]);
        std::vector<float> xs = cube.IntersectLocal(ray);
        REQUIRE(xs.size() == 2);
        REQUIRE(xs[0] == t1[i]);
        REQUIRE(xs[1] == t2[i]);
    }
}

TEST_CASE("A ray misses a cube", "[cubes]")
{
    Cube cube("cube");

    Tuple points[] = {Point(-2.f, 0.f, 0.f), Point(0.f, -2.f, 0.f), Point(0.f, 0.f, -2.f), Point(2.f, 0.f, 2.f),
                      Point(0.f, 2.f, 2.f),  Point(2.f, 0.f, -2.f), Point(0.f, 2.f, -2.f)};
    Tuple directions[] = {Vector(0.26726f, 0.53452f, 0.80178f),
                          Vector(0.80178f, 0.26726f, 0.53452f),
                          Vector(0.53452f, 0.80178f, 0.26726f),
                          Vector(0.f, 0.f, -1.f),
                          Vector(0.f, -1.f, 0.f),
                          Vector(-1.f, 0.f, 0.f),
                          Vector(-1.f, -1.f, 0.f)};
    for (int i = 0; i < 7; ++i)
    {
        Ray ray(points[i], directions[i]);
        std::vector<float> xs = cube.IntersectLocal(ray);
        REQUIRE(xs.empty());
    }
}

TEST_CASE("The normal on the surface of a cube", "[cubes]")
{
    Cube cube("cube");
    Tuple points[] = {Point(1.f, 0.5f, -0.8f),  Point(-1.f, -0.2f, 0.9f), Point(-0.4f, 1.f, -0.1f),
                      Point(0.3f, -1.f, -0.7f), Point(-0.6f, 0.3f, 1.f),  Point(0.4f, 0.4f, -1.f),
                      Point(1.f, 1.f, 1.f),     Point(-1.f, -1.f, -1.f)};
    Tuple normals[] = {Vector(1.f, 0.f, 0.f), Vector(-1.f, 0.f, 0.f), Vector(0.f, 1.f, 0.f), Vector(0.f, -1.f, 0.f),
                       Vector(0.f, 0.f, 1.f), Vector(0.f, 0.f, -1.f), Vector(1.f, 0.f, 0.f), Vector(-1.f, 0.f, 0.f)};
    for (int i = 0; i < 8; ++i)
    {
        Tuple normal = cube.NormalAtLocal(points[i]);
        REQUIRE(normal.IsEqual(normals[i]));
    }
}
