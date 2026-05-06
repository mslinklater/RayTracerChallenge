#include "intersection.hpp"
#include "maths.hpp"
#include "ray.hpp"
#include "shapes/cone.hpp"
#include "tuple.hpp"
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

TEST_CASE("Intersecting a cone with a ray", "[cones]")
{
    Cone cone("cone");

    struct TestCase
    {
        std::string name;
        Tuple origin;
        Tuple direction;
        float expectedT1;
        float expectedT2;
    };

    std::vector<TestCase> cases = {
        {"ray parallel to one half", Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f), 5.f, 5.f},
        {"ray parallel to other half", Point(0.f, 0.f, -5.f), Vector(1.f, 1.f, 1.f), 8.66025f, 8.66025f},
        {"ray at an angle", Point(1.f, 1.f, -5.f), Vector(-0.5f, -1.f, 1.f), 4.55006f, 49.44994f},
    };

    for (size_t i = 0; i < cases.size(); ++i)
    {
        const auto& testCase = cases[i];
        DYNAMIC_SECTION("case " << i << ": " << testCase.name)
        {
            Ray r(testCase.origin, testCase.direction.Normalize());
            auto xs = cone.IntersectLocal(r);
            REQUIRE(xs.size() == 2);
            REQUIRE(AreEqual(xs[0].GetT(), testCase.expectedT1));
            REQUIRE(AreEqual(xs[1].GetT(), testCase.expectedT2));
        }
    }
}

TEST_CASE("Intersecting a cone with a ray parallel to one of its halves", "[cones]")
{
    Cone cone("cone");
    Ray r(Point(0.f, 0.f, -1.f), Vector(0.f, 1.f, 1.f).Normalize());
    auto xs = cone.IntersectLocal(r);
    REQUIRE(xs.size() == 1);
    // REQUIRE(AreEqual(xs[0], 0.35355f));
    REQUIRE(AreEqual(xs[0].GetT(), 0.70711f));
}

TEST_CASE("Default values for a cone", "[cones]")
{
    Cone cone("cone");
    REQUIRE(cone.GetMinimum() == -std::numeric_limits<float>::infinity());
    REQUIRE(cone.GetMaximum() == std::numeric_limits<float>::infinity());
    REQUIRE(!cone.IsClosed());
}

TEST_CASE("Intersecting a cone's end caps", "[cones]")
{
    struct TestCase
    {
        std::string name;
        Tuple origin;
        Tuple direction;
        int expectedCount;
    };

    Cone cone("cone");
    cone.SetMinimum(-0.5f);
    cone.SetMaximum(0.5f);
    cone.SetClosed(true);

    std::vector<TestCase> cases = {
        {"one", Point(0.f, 0.f, -5.f), Vector(0.f, 1.f, 0.f), 0},
        {"two", Point(0.f, 0.f, -0.25f), Vector(0.f, 1.f, 1.f), 2},
        {"three", Point(0.f, 0.f, -0.25f), Vector(0.f, 1.f, 0.f), 4},
    };

    for (size_t i = 0; i < cases.size(); ++i)
    {
        const auto& testCase = cases[i];
        DYNAMIC_SECTION("case " << i << ": " << testCase.name)
        {
            Ray r(testCase.origin, testCase.direction.Normalize());
            auto xs = cone.IntersectLocal(r);
            REQUIRE(xs.size() == testCase.expectedCount);
        }
    }
}

TEST_CASE("Computing the normal vector on a cone", "[cones]")
{
    struct TestCase
    {
        std::string name;
        Tuple point;
        Tuple expectedNormal;
    };

    Cone cone("cone");
    const std::vector<TestCase> cases = {
        {"one", Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 0.f)},
        {"two", Point(1.f, 1.f, 1.f), Vector(1.f, -std::sqrt(2.f), 1.f)},
        {"three", Point(-1.f, -1.f, 0.f), Vector(-1.f, 1.f, 0.f)},
    };

    for (size_t i = 0; i < cases.size(); ++i)
    {
        const auto& testCase = cases[i];
        DYNAMIC_SECTION("case " << i << ": " << testCase.name)
        {
            Intersection intersection;
            auto normal = cone.NormalAtLocal(testCase.point, intersection);
            REQUIRE(normal == testCase.expectedNormal);
        }
    }
}
