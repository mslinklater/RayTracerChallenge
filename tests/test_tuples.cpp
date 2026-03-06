#include <catch2/catch_test_macros.hpp>
#include "tuple.hpp"

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

TEST_CASE("Test for tuple addition", "[basic]")
{
    Tuple t1{1.f, 2.f, 3.f, 0.f};
    Tuple t2{4.f, 5.f, 6.f, 1.f};
    Tuple result = t1 + t2;
    REQUIRE(result.x == 5.f);
    REQUIRE(result.y == 7.f);
    REQUIRE(result.z == 9.f);
    REQUIRE(result.w == 1.f);
}

TEST_CASE("Subtracting two points", "[basic]")
{
    Tuple p1 = Point(3.f, 2.f, 1.f);
    Tuple p2 = Point(5.f, 6.f, 7.f);
    Tuple result = p1 + Tuple{-p2.x, -p2.y, -p2.z, -p2.w};
    REQUIRE(result.x == -2.f);
    REQUIRE(result.y == -4.f);
    REQUIRE(result.z == -6.f);
    REQUIRE(result.w == 0.f);
}

TEST_CASE("Subtracting two vectors", "[basic]")
{
    Tuple v1 = Vector(3.f, 2.f, 1.f);
    Tuple v2 = Vector(5.f, 6.f, 7.f);
    Tuple result = v1 + Tuple{-v2.x, -v2.y, -v2.z, -v2.w};
    REQUIRE(result.x == -2.f);
    REQUIRE(result.y == -4.f);
    REQUIRE(result.z == -6.f);
    REQUIRE(result.w == 0.f);
}

TEST_CASE("Subtracting a vector from a point", "[basic]")
{
    Tuple p = Point(3.f, 2.f, 1.f);
    Tuple v = Vector(5.f, 6.f, 7.f);
    Tuple result = p + Tuple{-v.x, -v.y, -v.z, -v.w};
    REQUIRE(result.x == -2.f);
    REQUIRE(result.y == -4.f);
    REQUIRE(result.z == -6.f);
    REQUIRE(result.w == 1.f);
}

TEST_CASE("Negating a tuple", "[basic]")
{
    Tuple t{1.f, -2.f, 3.f, -4.f};
    Tuple result = -t;
    REQUIRE(result.x == -1.f);
    REQUIRE(result.y == 2.f);
    REQUIRE(result.z == -3.f);
    REQUIRE(result.w == 4.f);
}

TEST_CASE("Multiplying a tuple by a scalar", "[basic]")
{
    Tuple t{1.f, -2.f, 3.f, -4.f};
    Tuple result = t * 3.5f;
    REQUIRE(result.x == 3.5f);
    REQUIRE(result.y == -7.f);
    REQUIRE(result.z == 10.5f);
    REQUIRE(result.w == -14.f);
}

TEST_CASE("Dividing a tuple by a scalar", "[basic]")
{
    Tuple t{1.f, -2.f, 3.f, -4.f};
    Tuple result = t / 2.f;
    REQUIRE(result.x == 0.5f);
    REQUIRE(result.y == -1.f);
    REQUIRE(result.z == 1.5f);
    REQUIRE(result.w == -2.f);
}

TEST_CASE("Magnitude of vector(1, 0, 0)", "[basic]")
{
    Tuple v = Vector(1.f, 0.f, 0.f);
    REQUIRE(AreEqual(v.Magnitude(), 1.f));
}

TEST_CASE("Magnitude of vector(0, 1, 0)", "[basic]")
{
    Tuple v = Vector(0.f, 1.f, 0.f);
    REQUIRE(AreEqual(v.Magnitude(), 1.f));
}

TEST_CASE("Magnitude of vector(0, 0, 1)", "[basic]")
{
    Tuple v = Vector(0.f, 0.f, 1.f);
    REQUIRE(AreEqual(v.Magnitude(), 1.f));
}

TEST_CASE("Magnitude of vector(1, 2, 3)", "[basic]")
{
    Tuple v = Vector(1.f, 2.f, 3.f);
    REQUIRE(AreEqual(v.Magnitude(), std::sqrt(14.f)));
}

TEST_CASE("Magnitude of vector(-1, -2, -3)", "[basic]")
{
    Tuple v = Vector(-1.f, -2.f, -3.f);
    REQUIRE(AreEqual(v.Magnitude(), std::sqrt(14.f)));
}

TEST_CASE("Normalizing vector(4, 0, 0)", "[basic]")
{
    Tuple v = Vector(4.f, 0.f, 0.f);
    Tuple result = v.Normalize();
    REQUIRE(result.x == 1.f);
    REQUIRE(result.y == 0.f);
    REQUIRE(result.z == 0.f);
    REQUIRE(result.w == 0.f);
}

TEST_CASE("Normalizing vector(1, 2, 3)", "[basic]")
{
    Tuple v = Vector(1.f, 2.f, 3.f);
    Tuple result = v.Normalize();
    REQUIRE(AreEqual(result.x, 0.26726f));
    REQUIRE(AreEqual(result.y, 0.53452f));
    REQUIRE(AreEqual(result.z, 0.80178f));
    REQUIRE(result.w == 0.f);
}

TEST_CASE("Magnitude of a normalized vector(1, 2, 3)", "[basic]")
{
    Tuple v = Vector(1.f, 2.f, 3.f);
    Tuple n = v.Normalize();
    float mag = n.Magnitude();
    REQUIRE(AreEqual(mag, 1.f));
}

TEST_CASE("Normalizing a zero vector", "[basic]")
{
    Tuple v = Vector(0.f, 0.f, 0.f);
    Tuple result = v.Normalize();
    REQUIRE(result.x == 0.f);
    REQUIRE(result.y == 0.f);
    REQUIRE(result.z == 0.f);
    REQUIRE(result.w == 0.f);
}

TEST_CASE("Dot product of two vectors", "[basic]")
{
    Tuple v1 = Vector(1.f, 2.f, 3.f);
    Tuple v2 = Vector(2.f, 3.f, 4.f);
    float dot = v1 | v2;
    REQUIRE(AreEqual(dot, 20.f));
}

TEST_CASE("Dot product of two points", "[basic]")
{
    Tuple v1 = Point(1.f, 2.f, 3.f);
    Tuple v2 = Point(2.f, 3.f, 4.f);
    float dot = v1 | v2;
    REQUIRE(!AreEqual(dot, 20.f));
}

TEST_CASE("Cross product of two vectors", "[basic]")
{
    Tuple v1 = Vector(1.f, 2.f, 3.f);
    Tuple v2 = Vector(2.f, 3.f, 4.f);
    Tuple cross1 = v1 ^ v2;
    Tuple cross2 = v2 ^ v1;
    REQUIRE(cross1.IsEqual(Vector(-1.f, 2.f, -1.f)));
    REQUIRE(cross2.IsEqual(Vector(1.f, -2.f, 1.f)));
}