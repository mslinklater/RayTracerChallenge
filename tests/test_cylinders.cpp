#include "maths.hpp"
#include "ray.hpp"
#include "shapes/cylinder.hpp"
#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("A ray misses a cylinder", "[cylinders]")
{
    struct TestCase
    {
        std::string name;
        Tuple origin;
        Tuple direction;
    };

    Cylinder cyl("cylinder");
    const std::vector<TestCase> cases = {
        {"parallel from +x", Point(1.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)},
        {"parallel from +y axis", Point(0.0f, 1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)},
        {"diagonal past cylinder", Point(0.0f, 0.0f, -5.0f), Vector(1.0f, 1.0f, 1.0f)},
    };

    for (size_t i = 0; i < cases.size(); ++i)
    {
        const auto &testCase = cases[i];
        DYNAMIC_SECTION("case " << i << ": " << testCase.name)
        {
            Ray r(testCase.origin, testCase.direction);
            auto xs = cyl.IntersectLocal(r);
            REQUIRE(xs.empty());
        }
    }
}

TEST_CASE("A ray strikes a cylinder", "[cylinders]")
{
    struct TestCase
    {
        std::string name;
        Tuple origin;
        Tuple direction;
        float expectedT1;
        float expectedT2;
    };

    Cylinder cyl("cylinder");
    const std::vector<TestCase> cases = {
        {"tangent hit", Point(1.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f), 5.0f, 5.0f},
        {"straight through center", Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f), 4.0f, 6.0f},
        {"angled hit", Point(0.5f, 0.0f, -5.0f), Vector(0.1f, 1.0f, 1.0f), 6.80798f, 7.08872f},
    };

    for (size_t i = 0; i < cases.size(); ++i)
    {
        const auto &testCase = cases[i];
        DYNAMIC_SECTION("case " << i << ": " << testCase.name)
        {
            Ray r(testCase.origin, testCase.direction.Normalize());
            auto xs = cyl.IntersectLocal(r);
            REQUIRE(xs.size() == 2);
            REQUIRE(AreEqual(xs[0], testCase.expectedT1));
            REQUIRE(AreEqual(xs[1], testCase.expectedT2));
        }
    }
}

TEST_CASE("Normal vector on a cylinder", "[cylinders]")
{
    struct TestCase
    {
        std::string name;
        Tuple point;
        Tuple expectedNormal;
    };

    Cylinder cyl("cylinder");
    const std::vector<TestCase> cases = {
        {"point on +x", Point(1.0f, 0.0f, 0.0f), Vector(1.0f, 0.0f, 0.0f)},
        {"point on -z", Point(0.0f, 5.0f, -1.0f), Vector(0.0f, 0.0f, -1.0f)},
        {"point on +z", Point(0.0f, -2.0f, 1.0f), Vector(0.0f, 0.0f, 1.0f)},
        {"point on -x", Point(-1.0f, 1.0f, 0.0f), Vector(-1.0f, 0.0f, 0.0f)},
    };

    for (size_t i = 0; i < cases.size(); ++i)
    {
        const auto &testCase = cases[i];
        DYNAMIC_SECTION("case " << i << ": " << testCase.name)
        {
            Tuple n = cyl.NormalAtLocal(testCase.point);
            REQUIRE(n == testCase.expectedNormal);
        }
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
    struct TestCase
    {
        std::string name;
        Tuple origin;
        Tuple direction;
        size_t expectedCount;
    };

    Cylinder cyl("cylinder");
    cyl.SetMinimum(1.0f);
    cyl.SetMaximum(2.0f);
    const std::vector<TestCase> cases = {
        {"inside bounds but misses sides", Point(0.0f, 1.5f, 0.0f), Vector(0.1f, 1.0f, 0.0f), 0},
        {"above cylinder", Point(0.0f, 3.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f), 0},
        {"below cylinder", Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f), 0},
        {"starts above max", Point(0.0f, 2.5f, -5.0f), Vector(0.0f, 0.0f, 1.0f), 0},
        {"starts at min edge", Point(0.0f, 1.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f), 0},
        {"passes through constrained body", Point(0.0f, 1.5f, -2.0f), Vector(0.0f, 0.0f, 1.0f), 2},
    };

    for (size_t i = 0; i < cases.size(); ++i)
    {
        const auto &testCase = cases[i];
        DYNAMIC_SECTION("case " << i << ": " << testCase.name)
        {
            Ray r(testCase.origin, testCase.direction.Normalize());
            auto xs = cyl.IntersectLocal(r);
            REQUIRE(xs.size() == testCase.expectedCount);
        }
    }
}

TEST_CASE("The default closed value of a cylinder", "[cylinders]")
{
    Cylinder cyl("cylinder");
    REQUIRE(cyl.IsClosed() == false);
}

TEST_CASE("Intersecting the caps of a closed cylinder", "[cylinders]")
{
    struct TestCase
    {
        std::string name;
        Tuple origin;
        Tuple direction;
    };

    Cylinder cyl("cylinder");
    cyl.SetMinimum(1.0f);
    cyl.SetMaximum(2.0f);
    cyl.SetClosed(true);

    const std::vector<TestCase> cases = {
        {"one", Point(0.f, 3.f, 0.f), Vector(0.f, -1.f, 0.f)},
        {"two", Point(0.f, 3.f, -2.f), Vector(0.f, -1.f, 2.f)},
        {"three", Point(0.f, 4.f, -2.f), Vector(0.f, -1.f, 1.f)},
        {"four", Point(0.f, 0.f, -2.f), Vector(0.f, 1.f, 2.f)},
        {"five", Point(0.f, -1.f, -2.f), Vector(0.f, 1.f, 1.f)},
    };

    for (size_t i = 0; i < cases.size(); ++i)
    {
        const auto &testCase = cases[i];
        DYNAMIC_SECTION("case " << i << ": " << testCase.name)
        {
            Ray r(testCase.origin, testCase.direction.Normalize());
            auto xs = cyl.IntersectLocal(r);
            REQUIRE(xs.size() == 2);
        }
    }
}

TEST_CASE("The normal vector on a cylinder's end caps", "[cylinders]")
{
    Cylinder cyl("cylinder");
    cyl.SetMinimum(1.0f);
    cyl.SetMaximum(2.0f);
    cyl.SetClosed(true);

    struct TestCase
    {
        std::string name;
        Tuple point;
        Tuple normal;
    };

    const std::vector<TestCase> cases = {{"point on lower cap 1", Point(0.f, 1.f, 0.f), Vector(0.f, -1.f, 0.f)},
                                         {"point on lower cap 2", Point(0.5f, 1.f, 0.f), Vector(0.f, -1.f, 0.f)},
                                         {"point on lower cap 3", Point(0.f, 1.f, 0.5f), Vector(0.f, -1.f, 0.f)},
                                         {"point on upper cap 1", Point(0.f, 2.f, 0.f), Vector(0.f, 1.f, 0.f)},
                                         {"point on upper cap 2", Point(0.5f, 2.f, 0.f), Vector(0.f, 1.f, 0.f)},
                                         {"point on upper cap 3", Point(0.f, 2.f, 0.5f), Vector(0.f, 1.f, 0.f)}};

    for (size_t i = 0; i < cases.size(); ++i)
    {
        const auto &testCase = cases[i];
        DYNAMIC_SECTION("case " << i << ": " << testCase.name)
        {
            Tuple n = cyl.NormalAtLocal(testCase.point);
            REQUIRE(n == testCase.normal);
        }
    }
}
