#include <catch2/catch_test_macros.hpp>
#include "matrix.hpp"
#include "tuple.hpp"

TEST_CASE("Multiplying a point by a translation matrix", "[transformations]")
{
    Matrix translation = CreateTranslation(5.f, -3.f, 2.f);
    Tuple point = Point(-3.f, 4.f, 5.f);
    Tuple result = translation * point;
    REQUIRE(result == Point(2.f, 1.f, 7.f));
}

TEST_CASE("Multiplying a point by the inverse of a translation matrix", "[transformations]")
{
    Matrix translation = CreateTranslation(5.f, -3.f, 2.f);
    Matrix inverse = translation.GetInverse();
    Tuple point = Point(-3.f, 4.f, 5.f);
    Tuple result = inverse * point;
    REQUIRE(result == Point(-8.f, 7.f, 3.f));
}

TEST_CASE("Translation does not affect vectors", "[transformations]")
{
    Matrix translation = CreateTranslation(5.f, -3.f, 2.f);
    Tuple vector = Vector(-3.f, 4.f, 5.f);
    Tuple result = translation * vector;
    REQUIRE(result == vector);
}

TEST_CASE("Multiplying a point by a scaling matrix", "[transformations]")
{
    Matrix scaling = CreateScaling(2.f, 3.f, 4.f);
    Tuple point = Point(-4.f, 6.f, 8.f);
    Tuple result = scaling * point;
    REQUIRE(result == Point(-8.f, 18.f, 32.f));
}

TEST_CASE("Multiplying a vector by a scaling matrix", "[transformations]")
{
    Matrix scaling = CreateScaling(2.f, 3.f, 4.f);
    Tuple vector = Vector(-4.f, 6.f, 8.f);
    Tuple result = scaling * vector;
    REQUIRE(result == Vector(-8.f, 18.f, 32.f));
}

TEST_CASE("Multiplying by the inverse of a scaling matrix", "[transformations]")
{
    Matrix scaling = CreateScaling(2.f, 3.f, 4.f);
    Matrix inverse = scaling.GetInverse();
    Tuple vector = Vector(-4.f, 6.f, 8.f);
    Tuple result = inverse * vector;
    REQUIRE(result == Vector(-2.f, 2.f, 2.f));
}

TEST_CASE("Reflection is scaling by a negative value", "[transformations]")
{
    Matrix reflection = CreateScaling(-1.f, 1.f, 1.f);
    Tuple point = Point(2.f, 3.f, 4.f);
    Tuple result = reflection * point;
    REQUIRE(result == Point(-2.f, 3.f, 4.f));
}

TEST_CASE("Rotating a point around the x axis", "[transformations]")
{
    Matrix eighthRotation = CreateRotationX(M_PI / 4.f);
    Matrix quarterRotation = CreateRotationX(M_PI / 2.f);
    Tuple point = Point(0.f, 1.f, 0.f);
    Tuple eighthResult = eighthRotation * point;
    Tuple quarterResult = quarterRotation * point;
    REQUIRE(AreEqual(eighthResult.x, 0.f));
    REQUIRE(AreEqual(eighthResult.y, std::sqrt(2.f) / 2.f));
    REQUIRE(AreEqual(eighthResult.z, std::sqrt(2.f) / 2.f));
    REQUIRE(AreEqual(quarterResult.x, 0.f));
    REQUIRE(AreEqual(quarterResult.y, 0.f));
    REQUIRE(AreEqual(quarterResult.z, 1.f));
}