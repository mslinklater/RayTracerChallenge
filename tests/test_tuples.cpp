#include <catch2/catch_test_macros.hpp>
#include "tuples.hpp"

TEST_CASE("Create tuple", "[basic]")
{
    Tuple t{1.f, 2.f, 3.f, 1.f};
    REQUIRE(t.x == 1.f);
    REQUIRE(t.y == 2.f);
    REQUIRE(t.z == 3.f);
    REQUIRE(t.w == 1.f);
}

TEST_CASE("Test for tuple is point", "[basic]")
{
    Tuple t{4.f, -4.f, 3.f, 1.f};
    REQUIRE(t.IsPoint() == true);
}

TEST_CASE("Test for tuple is not point", "[basic]")
{
    Tuple t{4.f, -4.f, 3.f, 0.f};
    REQUIRE(t.IsPoint() == false);
}

TEST_CASE("Test for tuple is vector", "[basic]")
{
    Tuple t{4.f, -4.f, 3.f, 0.f};
    REQUIRE(t.IsVector() == true);
}

TEST_CASE("Test for tuple is not vector", "[basic]")
{
    Tuple t{4.f, -4.f, 3.f, 1.f};
    REQUIRE(t.IsVector() == false);
}

TEST_CASE("Test for Point creating Point", "[basic]")
{
    Tuple t = Point(4.f, -4.f, 3.f);
    REQUIRE(t.x == 4.f);
    REQUIRE(t.y == -4.f);
    REQUIRE(t.z == 3.f);
    REQUIRE(t.IsPoint() == true);
}

TEST_CASE("Test for Vector creating Vector", "[basic]")
{
    Tuple t = Vector(4.f, -4.f, 3.f);
    REQUIRE(t.x == 4.f);
    REQUIRE(t.y == -4.f);
    REQUIRE(t.z == 3.f);
    REQUIRE(t.IsVector() == true);
}

TEST_CASE("Test for tuple equality", "[basic]")
{
    Tuple t1{1.f, 2.f, 3.f, 1.f};
    Tuple t2{1.f, 2.f, 3.f, 1.f};
    REQUIRE(t1.IsEqual(t2) == true);
}

TEST_CASE("Test for tuple inequality", "[basic]")
{
    Tuple t1{1.f, 2.1f, 3.f, 1.f};
    Tuple t2{1.f, 2.f, 3.f, 1.f};
    REQUIRE(t1.IsEqual(t2) == false);
}