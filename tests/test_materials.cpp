#include <catch2/catch_test_macros.hpp>
#include "material.hpp"
#include "tuple.hpp"
#include "light.hpp"
#include "renderer.hpp"

TEST_CASE("Create a material and check default values", "[materials]")
{
    Material m = Material();

    REQUIRE(m.GetColor() == Color(1.f, 1.f, 1.f));
    REQUIRE(m.GetAmbient() == 0.1f);
    REQUIRE(m.GetDiffuse() == 0.9f);
    REQUIRE(m.GetSpecular() == 0.9f);
    REQUIRE(m.GetShininess() == 200.f);
}

//////////////////////////////////////////////////////////////////////////////
// Lighting fixture

struct LightingFixture
{
    LightingFixture()
        : material(Material()), position(Point(0.f, 0.f, 0.f))
    {
        // Shared setup logic runs before every fixture-based test.
    }

    ~LightingFixture()
    {
        // Shared teardown logic runs after every fixture-based test.
    }

    Material material;
    Tuple position;
};

TEST_CASE_METHOD(LightingFixture, "Lighting with the eye between the light and the surface", "[materials][fixture]")
{
    Tuple eyeVector = Vector(0.f, 0.f, -1.f);
    Tuple normalVector = Vector(0.f, 0.f, -1.f);
    Light light(Point(0.f, 0.f, -10.f), Color(1.f, 1.f, 1.f));

    Color result = Renderer::Lighting(material, light, position, eyeVector, normalVector, EInShadow::No);
    REQUIRE(result == Color(1.9f, 1.9f, 1.9f));
}

TEST_CASE_METHOD(LightingFixture, "Lighting with the surface in shadow", "[materials][fixture]")
{
    Tuple eyeVector = Vector(0.f, 0.f, -1.f);
    Tuple normalVector = Vector(0.f, 0.f, -1.f);
    Light light(Point(0.f, 0.f, -10.f), Color(1.f, 1.f, 1.f));

    Color result = Renderer::Lighting(material, light, position, eyeVector, normalVector, EInShadow::Yes);
    REQUIRE(result == Color(0.1f, 0.1f, 0.1f));
}

TEST_CASE_METHOD(LightingFixture, "Lighting with the eye between the light and the surface, eye offset 45 degrees", "[materials][fixture]")
{
    Tuple eyeVector = Vector(0.f, std::sqrt(2.f) / 2.f, -std::sqrt(2.f) / 2.f);
    Tuple normalVector = Vector(0.f, 0.f, -1.f);
    Light light(Point(0.f, 0.f, -10.f), Color(1.f, 1.f, 1.f));

    Color result = Renderer::Lighting(material, light, position, eyeVector, normalVector, EInShadow::No);
    REQUIRE(result == Color(1.0f, 1.0f, 1.0f));
}

TEST_CASE_METHOD(LightingFixture, "Lighting with the eye opposite the surface, light offset 45 degrees", "[materials][fixture]")
{
    Tuple eyeVector = Vector(0.f, 0.f, -1.f);
    Tuple normalVector = Vector(0.f, 0.f, -1.f);
    Light light(Point(0.f, 10.f, -10.f), Color(1.f, 1.f, 1.f));

    Color result = Renderer::Lighting(material, light, position, eyeVector, normalVector, EInShadow::No);
    REQUIRE(result == Color(0.7364f, 0.7364f, 0.7364f));
}

TEST_CASE_METHOD(LightingFixture, "Lighting with the eye in the path of the reflection vector", "[materials][fixture]")
{
    Tuple eyeVector = Vector(0.f, -std::sqrt(2.f) / 2.f, -std::sqrt(2.f) / 2.f);
    Tuple normalVector = Vector(0.f, 0.f, -1.f);
    Light light(Point(0.f, 10.f, -10.f), Color(1.f, 1.f, 1.f));

    Color result = Renderer::Lighting(material, light, position, eyeVector, normalVector, EInShadow::No);
    REQUIRE(result == Color(1.6364f, 1.6364f, 1.6364f));
}

TEST_CASE_METHOD(LightingFixture, "Lighting with the light behind the surface", "[materials][fixture]")
{
    Tuple eyeVector = Vector(0.f, 0.f, -1.f);
    Tuple normalVector = Vector(0.f, 0.f, -1.f);
    Light light(Point(0.f, 0.f, 10.f), Color(1.f, 1.f, 1.f));

    Color result = Renderer::Lighting(material, light, position, eyeVector, normalVector, EInShadow::No);
    REQUIRE(result == Color(0.1f, 0.1f, 0.1f));
}
