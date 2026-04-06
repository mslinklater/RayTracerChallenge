#include "light.hpp"
#include "material.hpp"
#include "patterns/stripe_pattern.hpp"
#include "renderer.hpp"
#include "sphere.hpp"
#include "tuple.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Create a material and check default values", "[materials]")
{
    Material m = Material();

    REQUIRE(m.GetColor() == Color(1.f, 1.f, 1.f));
    REQUIRE(m.GetAmbient() == 0.1f);
    REQUIRE(m.GetDiffuse() == 0.9f);
    REQUIRE(m.GetSpecular() == 0.9f);
    REQUIRE(m.GetShininess() == 200.f);
}

TEST_CASE("Create a material, set values and check updated values", "[materials]")
{
    Material m = Material();

    m.SetColor(Color(0.5f, 0.5f, 0.5f));
    m.SetAmbient(0.2f);
    m.SetDiffuse(0.8f);
    m.SetSpecular(0.7f);
    m.SetShininess(100.f);

    REQUIRE(m.GetColor() == Color(0.5f, 0.5f, 0.5f));
    REQUIRE(m.GetAmbient() == 0.2f);
    REQUIRE(m.GetDiffuse() == 0.8f);
    REQUIRE(m.GetSpecular() == 0.7f);
    REQUIRE(m.GetShininess() == 100.f);
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
