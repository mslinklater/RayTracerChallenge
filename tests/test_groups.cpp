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
    REQUIRE(g.GetChildren().empty());
}

TEST_CASE("A shape has a parent attribute", "[groups]")
{
    TestShape s("shape");
    REQUIRE(s.GetParent() == nullptr);
}
