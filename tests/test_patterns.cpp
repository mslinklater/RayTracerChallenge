#include "color.hpp"
#include "pattern.hpp"
#include "sphere.hpp"
#include <catch2/catch_test_macros.hpp>

class TestPattern : public Pattern
{
    Color ColorAt(const Tuple &point) override
    {
        return kColorWhite;
    }
    Color ColorAtObject(const Shape &object, const Tuple &point) override
    {
        return kColorWhite;
    }
};

TEST_CASE("Creating a stripe pattern", "[patterns]")
{
    StripePattern pattern(kColorWhite, kColorBlack);
    REQUIRE(pattern.GetA() == kColorWhite);
    REQUIRE(pattern.GetB() == kColorBlack);
}

TEST_CASE("A stripe pattern is constant in y", "[patterns]")
{
    StripePattern pattern(kColorWhite, kColorBlack);
    REQUIRE(pattern.ColorAt(Point(0.f, 0.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.ColorAt(Point(0.f, 1.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.ColorAt(Point(0.f, 2.f, 0.f)) == kColorWhite);
}

TEST_CASE("A stripe pattern is constant in z", "[patterns]")
{
    StripePattern pattern(kColorWhite, kColorBlack);
    REQUIRE(pattern.ColorAt(Point(0.f, 0.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.ColorAt(Point(0.f, 0.f, 1.f)) == kColorWhite);
    REQUIRE(pattern.ColorAt(Point(0.f, 0.f, 2.f)) == kColorWhite);
}

TEST_CASE("A stripe pattern alternates in x", "[patterns]")
{
    StripePattern pattern(kColorWhite, kColorBlack);
    REQUIRE(pattern.ColorAt(Point(0.f, 0.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.ColorAt(Point(0.9f, 0.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.ColorAt(Point(1.f, 0.f, 0.f)) == kColorBlack);
    REQUIRE(pattern.ColorAt(Point(-0.1f, 0.f, 0.f)) == kColorBlack);
    REQUIRE(pattern.ColorAt(Point(-1.f, 0.f, 0.f)) == kColorBlack);
    REQUIRE(pattern.ColorAt(Point(-1.1f, 0.f, 0.f)) == kColorWhite);
}

TEST_CASE("Stripes with an object transformation", "[patterns]")
{
    Sphere object = Sphere("object");
    object.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    StripePattern pattern(kColorWhite, kColorBlack);
    object.GetMutableMaterial().SetPattern(pattern);
    Color c = object.GetMutableMaterial().GetPattern()->ColorAtObject(object, Point(1.5f, 0.f, 0.f));
    REQUIRE(c == kColorWhite);
}

TEST_CASE("Stripes with a pattern transformation", "[patterns]")
{
    Sphere object = Sphere("object");
    StripePattern pattern(kColorWhite, kColorBlack);
    pattern.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    Color c = pattern.ColorAtObject(object, Point(1.5f, 0.f, 0.f));
    REQUIRE(c == kColorWhite);
}

TEST_CASE("Stripes with both an object and a pattern transformation", "[patterns]")
{
    Sphere object = Sphere("object");
    object.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    StripePattern pattern(kColorWhite, kColorBlack);
    pattern.SetTransform(Matrix::CreateTranslation(0.5f, 0.f, 0.f));
    Color c = pattern.ColorAtObject(object, Point(2.5f, 0.f, 0.f));
    REQUIRE(c == kColorWhite);
}

TEST_CASE("The default pattern transformation", "[patterns]")
{
    TestPattern pattern;
    Matrix expected = Matrix(4);
    expected.SetIdentity();
    REQUIRE(pattern.GetTransform() == expected);
}

TEST_CASE("Assigning a transformation", "[patterns]")
{
    TestPattern pattern;
    pattern.SetTransform(Matrix::CreateTranslation(1.f, 2.f, 3.f));
    REQUIRE(pattern.GetTransform() == Matrix::CreateTranslation(1.f, 2.f, 3.f));
}
