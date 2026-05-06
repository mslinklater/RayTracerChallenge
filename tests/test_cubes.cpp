#include "ray.hpp"
#include "shapes/cube.hpp"
#include "tuple.hpp"
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

TEST_CASE("A ray intersects a cube", "[cubes]")
{
    struct TestCase
    {
        std::string name;
        Tuple origin;
        Tuple direction;
        float expectedT1;
        float expectedT2;
    };

    Cube cube("cube");
    const std::vector<TestCase> cases = {
        {"from +x", Point(5.f, 0.5f, 0.f), Vector(-1.f, 0.f, 0.f), 4.f, 6.f},
        {"from -x", Point(-5.f, 0.5f, 0.f), Vector(1.f, 0.f, 0.f), 4.f, 6.f},
        {"from +y", Point(0.5f, 5.f, 0.f), Vector(0.f, -1.f, 0.f), 4.f, 6.f},
        {"from -y", Point(0.5f, -5.f, 0.f), Vector(0.f, 1.f, 0.f), 4.f, 6.f},
        {"from +z", Point(0.5f, 0.f, 5.f), Vector(0.f, 0.f, -1.f), 4.f, 6.f},
        {"from -z", Point(0.5f, 0.f, -5.f), Vector(0.f, 0.f, 1.f), 4.f, 6.f},
        {"from inside", Point(0.f, 0.5f, 0.f), Vector(0.f, 0.f, 1.f), -1.f, 1.f},
    };

    for (size_t i = 0; i < cases.size(); ++i)
    {
        const auto& testCase = cases[i];
        DYNAMIC_SECTION("case " << i << ": " << testCase.name)
        {
            Ray ray(testCase.origin, testCase.direction);
            std::vector<Intersection> xs = cube.IntersectLocal(ray);
            REQUIRE(xs.size() == 2);
            REQUIRE(xs[0].GetT() == testCase.expectedT1);
            REQUIRE(xs[1].GetT() == testCase.expectedT2);
        }
    }
}

TEST_CASE("A ray misses a cube", "[cubes]")
{
    struct TestCase
    {
        std::string name;
        Tuple origin;
        Tuple direction;
    };

    Cube cube("cube");
    const std::vector<TestCase> cases = {
        {"from -x corner", Point(-2.f, 0.f, 0.f), Vector(0.26726f, 0.53452f, 0.80178f)},
        {"from -y corner", Point(0.f, -2.f, 0.f), Vector(0.80178f, 0.26726f, 0.53452f)},
        {"from -z corner", Point(0.f, 0.f, -2.f), Vector(0.53452f, 0.80178f, 0.26726f)},
        {"past +x+z edge", Point(2.f, 0.f, 2.f), Vector(0.f, 0.f, -1.f)},
        {"past +y+z edge", Point(0.f, 2.f, 2.f), Vector(0.f, -1.f, 0.f)},
        {"past +x-z edge", Point(2.f, 0.f, -2.f), Vector(-1.f, 0.f, 0.f)},
        {"diagonal away", Point(0.f, 2.f, -2.f), Vector(-1.f, -1.f, 0.f)},
    };

    for (size_t i = 0; i < cases.size(); ++i)
    {
        const auto& testCase = cases[i];
        DYNAMIC_SECTION("case " << i << ": " << testCase.name)
        {
            Ray ray(testCase.origin, testCase.direction);
            std::vector<Intersection> xs = cube.IntersectLocal(ray);
            REQUIRE(xs.empty());
        }
    }
}

TEST_CASE("The normal on the surface of a cube", "[cubes]")
{
    struct TestCase
    {
        std::string name;
        Tuple point;
        Tuple expectedNormal;
    };

    Cube cube("cube");
    const std::vector<TestCase> cases = {
        {"+x face", Point(1.f, 0.5f, -0.8f), Vector(1.f, 0.f, 0.f)},
        {"-x face", Point(-1.f, -0.2f, 0.9f), Vector(-1.f, 0.f, 0.f)},
        {"+y face", Point(-0.4f, 1.f, -0.1f), Vector(0.f, 1.f, 0.f)},
        {"-y face", Point(0.3f, -1.f, -0.7f), Vector(0.f, -1.f, 0.f)},
        {"+z face", Point(-0.6f, 0.3f, 1.f), Vector(0.f, 0.f, 1.f)},
        {"-z face", Point(0.4f, 0.4f, -1.f), Vector(0.f, 0.f, -1.f)},
        {"+x corner", Point(1.f, 1.f, 1.f), Vector(1.f, 0.f, 0.f)},
        {"-x corner", Point(-1.f, -1.f, -1.f), Vector(-1.f, 0.f, 0.f)},
    };

    for (size_t i = 0; i < cases.size(); ++i)
    {
        const auto& testCase = cases[i];
        DYNAMIC_SECTION("case " << i << ": " << testCase.name)
        {
            Intersection intersection;
            Tuple normal = cube.NormalAtLocal(testCase.point, intersection);
            REQUIRE(normal.IsEqual(testCase.expectedNormal));
        }
    }
}
