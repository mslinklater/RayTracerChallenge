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

TEST_CASE("Evaluating the rule for a CSG operation", "[csg]")
{
    struct TestCase
    {
        CSG::Operation op;
        bool lhit;
        bool inl;
        bool inr;
        bool result;
    };

    TestCase testCases[] = {
        {CSG::OpUnion, true, true, true, false},          {CSG::OpUnion, true, true, false, true},
        {CSG::OpUnion, true, false, true, false},         {CSG::OpUnion, true, false, false, true},
        {CSG::OpUnion, false, true, true, false},         {CSG::OpUnion, false, true, false, false},
        {CSG::OpUnion, false, false, true, true},         {CSG::OpUnion, false, false, false, true},

        {CSG::OpIntersection, true, true, true, true},    {CSG::OpIntersection, true, true, false, false},
        {CSG::OpIntersection, true, false, true, true},   {CSG::OpIntersection, true, false, false, false},
        {CSG::OpIntersection, false, true, true, true},   {CSG::OpIntersection, false, true, false, true},
        {CSG::OpIntersection, false, false, true, false}, {CSG::OpIntersection, false, false, false, false},

        {CSG::OpDifference, true, true, true, false},     {CSG::OpDifference, true, true, false, true},
        {CSG::OpDifference, true, false, true, false},    {CSG::OpDifference, true, false, false, true},
        {CSG::OpDifference, false, true, true, true},     {CSG::OpDifference, false, true, false, true},
        {CSG::OpDifference, false, false, true, false},   {CSG::OpDifference, false, false, false, false}};

    for (auto& c : testCases)
    {
        bool result = CSG::IntersectionAllowed(c.op, c.lhit, c.inl, c.inr);
        REQUIRE(result == c.result);
    }
}
