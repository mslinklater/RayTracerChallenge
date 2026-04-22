#include "matrix.hpp"
#include "shapes/group.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Creating a new group", "[groups]")
{
    Group g("group");
    Matrix identity(4);
    identity.SetIdentity();
    REQUIRE(g.GetTransform() == identity);
    REQUIRE(g.GetChildren().empty());
}
