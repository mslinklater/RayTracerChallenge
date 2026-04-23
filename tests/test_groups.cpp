#include "matrix.hpp"
#include "shapes/group.hpp"
#include "test_shapes.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Creating a new group", "[groups]")
{
    Group g("group");
    Matrix identity(4);
    identity.SetIdentity();
    REQUIRE(g.GetTransform() == identity);
    REQUIRE(g.GetNumChildren() == 0);
}

TEST_CASE("A shape has a parent attribute", "[groups]")
{
    TestShape s("shape");
    REQUIRE(s.GetParent() == nullptr);
}

TEST_CASE("Adding a child to a group", "[groups]")
{
    Group g("group");
    TestShape s("shape");
    g.AddChild(&s);
    REQUIRE(g.GetNumChildren() == 1);
    REQUIRE(g.Contains(&s));
    REQUIRE(s.GetParent() == &g); // TODO: set parent when adding to group
}
