#include <catch2/catch_test_macros.hpp>
#include "matrix.hpp"
#include "tuple.hpp"

TEST_CASE("Multiplying a point by a translation matrix", "[transformations]")
{
    Matrix translation = Matrix::CreateTranslation(5.f, -3.f, 2.f);
    Tuple point = Point(-3.f, 4.f, 5.f);
    Tuple result = translation * point;
    REQUIRE(result == Point(2.f, 1.f, 7.f));
}

TEST_CASE("Multiplying a point by the inverse of a translation matrix", "[transformations]")
{
    Matrix translation = Matrix::CreateTranslation(5.f, -3.f, 2.f);
    Matrix inverse = translation.GetInverse();
    Tuple point = Point(-3.f, 4.f, 5.f);
    Tuple result = inverse * point;
    REQUIRE(result == Point(-8.f, 7.f, 3.f));
}

TEST_CASE("Translation does not affect vectors", "[transformations]")
{
    Matrix translation = Matrix::CreateTranslation(5.f, -3.f, 2.f);
    Tuple vector = Vector(-3.f, 4.f, 5.f);
    Tuple result = translation * vector;
    REQUIRE(result == vector);
}

TEST_CASE("Multiplying a point by a scaling matrix", "[transformations]")
{
    Matrix scaling = Matrix::CreateScaling(2.f, 3.f, 4.f);
    Tuple point = Point(-4.f, 6.f, 8.f);
    Tuple result = scaling * point;
    REQUIRE(result == Point(-8.f, 18.f, 32.f));
}

TEST_CASE("Multiplying a vector by a scaling matrix", "[transformations]")
{
    Matrix scaling = Matrix::CreateScaling(2.f, 3.f, 4.f);
    Tuple vector = Vector(-4.f, 6.f, 8.f);
    Tuple result = scaling * vector;
    REQUIRE(result == Vector(-8.f, 18.f, 32.f));
}

TEST_CASE("Multiplying by the inverse of a scaling matrix", "[transformations]")
{
    Matrix scaling = Matrix::CreateScaling(2.f, 3.f, 4.f);
    Matrix inverse = scaling.GetInverse();
    Tuple vector = Vector(-4.f, 6.f, 8.f);
    Tuple result = inverse * vector;
    REQUIRE(result == Vector(-2.f, 2.f, 2.f));
}

TEST_CASE("Reflection is scaling by a negative value", "[transformations]")
{
    Matrix reflection = Matrix::CreateScaling(-1.f, 1.f, 1.f);
    Tuple point = Point(2.f, 3.f, 4.f);
    Tuple result = reflection * point;
    REQUIRE(result == Point(-2.f, 3.f, 4.f));
}

TEST_CASE("Rotating a point around the x axis", "[transformations]")
{
    Matrix eighthRotation = Matrix::CreateRotationX(M_PI / 4.f);
    Matrix quarterRotation = Matrix::CreateRotationX(M_PI / 2.f);
    Tuple point = Point(0.f, 1.f, 0.f);
    Tuple eighthResult = eighthRotation * point;
    Tuple quarterResult = quarterRotation * point;

    REQUIRE(eighthResult == Point(0.f, std::sqrt(2.f) / 2.f, std::sqrt(2.f) / 2.f));
    REQUIRE(quarterResult == Point(0.f, 0.f, 1.f));
}

TEST_CASE("Rotating a point around the y axis", "[transformations]")
{
    Matrix eighthRotation = Matrix::CreateRotationY(M_PI / 4.f);
    Matrix quarterRotation = Matrix::CreateRotationY(M_PI / 2.f);
    Tuple point = Point(0.f, 0.f, 1.f);
    Tuple eighthResult = eighthRotation * point;
    Tuple quarterResult = quarterRotation * point;

    REQUIRE(eighthResult == Point(std::sqrt(2.f) / 2.f, 0.f, std::sqrt(2.f) / 2.f));
    REQUIRE(quarterResult == Point(1.f, 0.f, 0.f));
}

TEST_CASE("Rotating a point around the z axis", "[transformations]")
{
    Matrix eighthRotation = Matrix::CreateRotationZ(M_PI / 4.f);
    Matrix quarterRotation = Matrix::CreateRotationZ(M_PI / 2.f);
    Tuple point = Point(0.f, 1.f, 0.f);
    Tuple eighthResult = eighthRotation * point;
    Tuple quarterResult = quarterRotation * point;

    REQUIRE(eighthResult == Point(-std::sqrt(2.f) / 2.f, std::sqrt(2.f) / 2.f, 0.0f));
    REQUIRE(quarterResult == Point(-1.f, 0.f, 0.f));
}

TEST_CASE("Shearing transformation moves x in proportion to y", "[transformations]")
{
    Matrix shearing = Matrix::CreateShearing(1.f, 0.f, 0.f, 0.f, 0.f, 0.f);
    Tuple point = Point(2.f, 3.f, 4.f);
    Tuple result = shearing * point;
    REQUIRE(result == Point(5.f, 3.f, 4.f));
}

TEST_CASE("Shearing transformation moves x in proportion to z", "[transformations]")
{
    Matrix shearing = Matrix::CreateShearing(0.f, 1.f, 0.f, 0.f, 0.f, 0.f);
    Tuple point = Point(2.f, 3.f, 4.f);
    Tuple result = shearing * point;
    REQUIRE(result == Point(6.f, 3.f, 4.f));
}

TEST_CASE("Shearing transformation moves y in proportion to x", "[transformations]")
{
    Matrix shearing = Matrix::CreateShearing(0.f, 0.f, 1.f, 0.f, 0.f, 0.f);
    Tuple point = Point(2.f, 3.f, 4.f);
    Tuple result = shearing * point;
    REQUIRE(result == Point(2.f, 5.f, 4.f));
}

TEST_CASE("Shearing transformation moves y in proportion to z", "[transformations]")
{
    Matrix shearing = Matrix::CreateShearing(0.f, 0.f, 0.f, 1.f, 0.f, 0.f);
    Tuple point = Point(2.f, 3.f, 4.f);
    Tuple result = shearing * point;
    REQUIRE(result == Point(2.f, 7.f, 4.f));
}

TEST_CASE("Shearing transformation moves z in proportion to x", "[transformations]")
{
    Matrix shearing = Matrix::CreateShearing(0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
    Tuple point = Point(2.f, 3.f, 4.f);
    Tuple result = shearing * point;
    REQUIRE(result == Point(2.f, 3.f, 6.f));
}

TEST_CASE("Shearing transformation moves z in proportion to y", "[transformations]")
{
    Matrix shearing = Matrix::CreateShearing(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
    Tuple point = Point(2.f, 3.f, 4.f);
    Tuple result = shearing * point;
    REQUIRE(result == Point(2.f, 3.f, 7.f));
}

TEST_CASE("Individual transformations are applied in sequence", "[transformations]")
{
    Tuple point = Point(1.f, 0.f, 1.f);
    Matrix rotation = Matrix::CreateRotationX(M_PI / 2.f);
    Matrix scaling = Matrix::CreateScaling(5.f, 5.f, 5.f);
    Matrix translation = Matrix::CreateTranslation(10.f, 5.f, 7.f);

    Tuple rotatedPoint = rotation * point;
    REQUIRE(rotatedPoint == Point(1.f, -1.f, 0.f));

    Tuple scaledPoint = scaling * rotatedPoint;
    REQUIRE(scaledPoint == Point(5.f, -5.f, 0.f));

    Tuple translatedPoint = translation * scaledPoint;
    REQUIRE(translatedPoint == Point(15.f, 0.f, 7.f));
}

TEST_CASE("The transformation matrix for the default orientation", "[transformations]")
{
    Tuple from = Point(0.f, 0.f, 0.f);
    Tuple to = Point(0.f, 0.f, -1.f);
    Tuple up = Tuple(0.f, 1.f, 0.f);
    Matrix t = Matrix::ViewTransform(from, to, up);
    Matrix identity(4);
    identity.SetIdentity();
    REQUIRE(t == identity);
}

TEST_CASE("A view transformation matrix looking in positive z direction", "[transformations]")
{
    Tuple from = Point(0.f, 0.f, 0.f);
    Tuple to = Point(0.f, 0.f, 1.f);
    Tuple up = Tuple(0.f, 1.f, 0.f);
    Matrix t = Matrix::ViewTransform(from, to, up);
    REQUIRE(t == Matrix::CreateScaling(-1.f, 1.f, -1.f));
}

TEST_CASE("The view transformation moves the world", "[transformations]")
{
    Tuple from = Point(0.f, 0.f, 8.f);
    Tuple to = Point(0.f, 0.f, 0.f);
    Tuple up = Tuple(0.f, 1.f, 0.f);
    Matrix t = Matrix::ViewTransform(from, to, up);
    REQUIRE(t == Matrix::CreateTranslation(0.f, 0.f, -8.f));
}

TEST_CASE("An arbitrary view transformation", "[transformations]")
{
    Tuple from = Point(1.f, 3.f, 2.f);
    Tuple to = Point(4.f, -2.f, 8.f);
    Tuple up = Tuple(1.f, 1.f, 0.f);
    Matrix t = Matrix::ViewTransform(from, to, up);
    REQUIRE(t == Matrix(
                     {-0.50709f, 0.50709f, 0.67612f, -2.36643f,
                      0.76772f, 0.60609f, 0.12122f, -2.82843f,
                      -0.35857f, 0.59761f, -0.71714f, 0.f,
                      0.f, 0.f, 0.f, 1.f}));
}