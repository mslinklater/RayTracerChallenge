#include "shapes/csg.hpp"
#include "shapes/cube.hpp"
#include "shapes/sphere.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("CSG is created with an operation and two shapes", "[csg]")
{
    Sphere s1("s1");
    Cube s2("s2");
    CSG csg("csg", CSG::OpUnion, &s1, &s2);

    REQUIRE(csg.GetOperation() == CSG::OpUnion);
    REQUIRE(csg.GetLeft()->GetName() == "s1");
    REQUIRE(csg.GetRight()->GetName() == "s2");
    REQUIRE(s1.GetParent() == &csg);
    REQUIRE(s2.GetParent() == &csg);
}
