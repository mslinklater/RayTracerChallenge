#include "light.hpp"
#include "material.hpp"
#include "patterns/stripe_pattern.hpp"
#include "renderer.hpp"
#include "shapes/sphere.hpp"
#include "tuple.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Create a material and check default values", "[materials]")
{
    Material m = Material();

    REQUIRE(m.GetColor() == Material::kDefaultColor);
    REQUIRE(m.GetAmbient() == Material::kDefaultAmbient);
    REQUIRE(m.GetDiffuse() == Material::kDefaultDiffuse);
    REQUIRE(m.GetSpecular() == Material::kDefaultSpecular);
    REQUIRE(m.GetShininess() == Material::kDefaultShininess);
    REQUIRE(m.GetReflective() == Material::kDefaultReflective);
    REQUIRE(m.GetTransparency() == Material::kDefaultTransparency);
    REQUIRE(m.GetRefractiveIndex() == Material::kDefaultRefractiveIndex);
}

TEST_CASE("Create a material, set values and check updated values", "[materials]")
{
    Material m = Material();

    m.SetColor(Color(0.5f, 0.5f, 0.5f));
    m.SetAmbient(0.2f);
    m.SetDiffuse(0.8f);
    m.SetSpecular(0.7f);
    m.SetShininess(100.f);
    m.SetReflective(0.5f);
    m.SetTransparency(0.5f);
    m.SetRefractiveIndex(1.5f);

    REQUIRE(m.GetColor() == Color(0.5f, 0.5f, 0.5f));
    REQUIRE(m.GetAmbient() == 0.2f);
    REQUIRE(m.GetDiffuse() == 0.8f);
    REQUIRE(m.GetSpecular() == 0.7f);
    REQUIRE(m.GetShininess() == 100.f);
    REQUIRE(m.GetReflective() == 0.5f);
    REQUIRE(m.GetTransparency() == 0.5f);
    REQUIRE(m.GetRefractiveIndex() == 1.5f);
}

TEST_CASE("Lighting with a pattern applied", "[materials]")
{
    Material m = Material();
    StripePattern pattern = StripePattern(Color(1.f, 1.f, 1.f), Color(0.f, 0.f, 0.f));
    m.SetPattern(pattern);
    m.SetAmbient(1.f);
    m.SetDiffuse(0.f);
    m.SetSpecular(0.f);
    Tuple eyeVector = Vector(0.f, 0.f, -1.f);
    Tuple normalVector = Vector(0.f, 0.f, -1.f);
    Light light = Light(Point(0.f, 0.f, -10.f), Color(1.f, 1.f, 1.f));
    Sphere s("test");
    Color c1 = Renderer::Lighting(m, s, light, Point(0.9f, 0.f, 0.f), eyeVector, normalVector, EInShadow::No);
    Color c2 = Renderer::Lighting(m, s, light, Point(1.1f, 0.f, 0.f), eyeVector, normalVector, EInShadow::No);
    REQUIRE(c1 == Color(1.f, 1.f, 1.f));
    REQUIRE(c2 == Color(0.f, 0.f, 0.f));
}

TEST_CASE("Materials compare all scalar properties", "[materials]")
{
    Material lhs;
    Material rhs;

    rhs.SetReflective(0.25f);
    REQUIRE_FALSE(lhs == rhs);

    rhs.SetReflective(lhs.GetReflective());
    rhs.SetTransparency(0.75f);
    REQUIRE_FALSE(lhs == rhs);

    rhs.SetTransparency(lhs.GetTransparency());
    rhs.SetRefractiveIndex(1.25f);
    REQUIRE_FALSE(lhs == rhs);
}

TEST_CASE("Materials compare patterns by value", "[materials]")
{
    Material lhs;
    Material rhs;
    StripePattern lhsPattern(kColorWhite, kColorBlack);
    StripePattern rhsPattern(kColorWhite, kColorBlack);

    lhsPattern.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    rhsPattern.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    lhs.SetPattern(lhsPattern);
    rhs.SetPattern(rhsPattern);

    REQUIRE(lhs == rhs);

    rhsPattern.SetTransform(Matrix::CreateTranslation(1.f, 0.f, 0.f));
    rhs.SetPattern(rhsPattern);
    REQUIRE_FALSE(lhs == rhs);
}

TEST_CASE("Copying a material clones its pattern", "[materials]")
{
    Material original;
    StripePattern pattern(kColorWhite, kColorBlack);
    original.SetPattern(pattern);

    Material copy = original;
    original.GetMutablePattern()->SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));

    REQUIRE(original.GetPattern() != nullptr);
    REQUIRE(copy.GetPattern() != nullptr);
    REQUIRE_FALSE(original == copy);
    Matrix expected(4);
    expected.SetIdentity();
    REQUIRE(copy.GetPattern()->GetTransform() == expected);
}
