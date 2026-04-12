#include "ray.hpp"
#include <catch2/catch_test_macros.hpp>

#if 0
TEST_CASE("A ray intersects a cube", "[cubes]")
{
    Cube cube("cube");

    Point points[] = {Point(5.f, 0.5f, 0.f), Point(-5.f, 0.5f, 0.f), Point(0.5f, 5.f, 0.f), Point(0.5f, -5.f, 0.f),
                      Point(0.5f, 0.f, 5.f), Point(0.5f, 0.f, -5.f), Point(0.f, 0.5f, 0.f)};
    Vector directions[] = {Vector(-1.f, 0.f, 0.f), Vector(1.f, 0.f, 0.f), Vector(0.f, -1.f, 0.f), Vector(0.f, 1.f, 0.f),
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
#endif
