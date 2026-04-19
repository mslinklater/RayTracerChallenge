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
        const auto &testCase = cases[i];
        DYNAMIC_SECTION("case " << i << ": " << testCase.name)
        {
            Ray r(testCase.origin, testCase.direction.Normalize());
            auto xs = cone.IntersectLocal(r);
            REQUIRE(xs.size() == 2);
            REQUIRE(AreEqual(xs[0], testCase.expectedT1));
            REQUIRE(AreEqual(xs[1], testCase.expectedT2));
        }
    }
}
