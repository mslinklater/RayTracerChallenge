#include "color.hpp"
#include "patterns/gradient_pattern.hpp"
#include "patterns/stripe_pattern.hpp"
#include "sphere.hpp"
#include <catch2/catch_test_macros.hpp>
#include <sys/_types/_mbstate_t.h>

class TestPattern : public Pattern
{
  public:
    Color PatternAt(const Tuple &point) override
    {
        return Color(point.x, point.y, point.z);
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
    REQUIRE(pattern.PatternAt(Point(0.f, 0.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.PatternAt(Point(0.f, 1.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.PatternAt(Point(0.f, 2.f, 0.f)) == kColorWhite);
}

TEST_CASE("A stripe pattern is constant in z", "[patterns]")
{
    StripePattern pattern(kColorWhite, kColorBlack);
    REQUIRE(pattern.PatternAt(Point(0.f, 0.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.PatternAt(Point(0.f, 0.f, 1.f)) == kColorWhite);
    REQUIRE(pattern.PatternAt(Point(0.f, 0.f, 2.f)) == kColorWhite);
}

TEST_CASE("A stripe pattern alternates in x", "[patterns]")
{
    StripePattern pattern(kColorWhite, kColorBlack);
    REQUIRE(pattern.PatternAt(Point(0.f, 0.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.PatternAt(Point(0.9f, 0.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.PatternAt(Point(1.f, 0.f, 0.f)) == kColorBlack);
    REQUIRE(pattern.PatternAt(Point(-0.1f, 0.f, 0.f)) == kColorBlack);
    REQUIRE(pattern.PatternAt(Point(-1.f, 0.f, 0.f)) == kColorBlack);
    REQUIRE(pattern.PatternAt(Point(-1.1f, 0.f, 0.f)) == kColorWhite);
}

TEST_CASE("Stripes with an object transformation", "[patterns]")
{
    Sphere object = Sphere("object");
    object.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    StripePattern pattern(kColorWhite, kColorBlack);
    object.GetMutableMaterial().SetPattern(pattern);
    Color c = object.GetMutableMaterial().GetPattern()->PatternAtShape(object, Point(1.5f, 0.f, 0.f));
    REQUIRE(c == kColorWhite);
}

TEST_CASE("Stripes with a pattern transformation", "[patterns]")
{
    Sphere object = Sphere("object");
    StripePattern pattern(kColorWhite, kColorBlack);
    pattern.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    Color c = pattern.PatternAtShape(object, Point(1.5f, 0.f, 0.f));
    REQUIRE(c == kColorWhite);
}

TEST_CASE("Stripes with both an object and a pattern transformation", "[patterns]")
{
    Sphere object = Sphere("object");
    object.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    StripePattern pattern(kColorWhite, kColorBlack);
    pattern.SetTransform(Matrix::CreateTranslation(0.5f, 0.f, 0.f));
    Color c = pattern.PatternAtShape(object, Point(2.5f, 0.f, 0.f));
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
TEST_CASE("A pattern with an object transformation", "[patterns]")
{
    Sphere shape = Sphere("shape");
    shape.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    TestPattern pattern;
    Color c = pattern.PatternAtShape(shape, Point(2.f, 3.f, 4.f));
    REQUIRE(c == Color(1.f, 1.5f, 2.f));
}

TEST_CASE("A pattern with a pattern transformation", "[patterns]")
{
    Sphere shape = Sphere("shape");
    TestPattern pattern;
    pattern.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    Color c = pattern.PatternAtShape(shape, Point(2.f, 3.f, 4.f));
    REQUIRE(c == Color(1.f, 1.5f, 2.f));
}

TEST_CASE("A pattern with both an ibject and a pattern transformation", "[patterns]")
{
    Sphere shape = Sphere("shape");
    shape.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    TestPattern pattern;
    pattern.SetTransform(Matrix::CreateTranslation(0.5f, 1.f, 1.5f));
    Color c = pattern.PatternAtShape(shape, Point(2.5f, 3.f, 3.5f));
    REQUIRE(c == Color(0.75f, 0.5f, 0.25f));
}

TEST_CASE("A gradient linearly interpolates between colors", "[patterns]")
{
    GradientPattern pattern(kColorWhite, kColorBlack);
    REQUIRE(pattern.PatternAt(Point(0.f, 0.f, 0.f)) == kColorWhite);
    REQUIRE(pattern.PatternAt(Point(0.25f, 0.f, 0.f)) == Color(0.75f, 0.75f, 0.75f));
    REQUIRE(pattern.PatternAt(Point(0.5f, 0.f, 0.f)) == Color(0.5f, 0.5f, 0.5f));
    REQUIRE(pattern.PatternAt(Point(0.75f, 0.f, 0.f)) == Color(0.25f, 0.25f, 0.25f));
}
