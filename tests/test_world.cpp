#include <catch2/catch_test_macros.hpp>
#include "world.hpp"

World DefaultWorld()
{
    World w;
    Light light = Light(Point(-10.f, 10.f, -10.f), Color(1.f, 1.f, 1.f));
    w.AddLight(light);
    Sphere s1;
    s1.GetMutableMaterial().SetColor(Color(0.8f, 1.f, 0.6f));
    s1.GetMutableMaterial().SetDiffuse(0.7f);
    s1.GetMutableMaterial().SetSpecular(0.2f);
    w.AddObject(s1);
    Sphere s2;
    s2.SetTransform(Matrix::CreateScaling(0.5f, 0.5f, 0.5f));
    w.AddObject(s2);
    return w;
}

TEST_CASE("Creating a world", "[world]")
{
    World world;
    REQUIRE(world.GetObjects().empty());
    REQUIRE(world.GetLights().empty());
}

TEST_CASE("The default world")
{
    World w = DefaultWorld();
    Light light = Light(Point(-10.f, 10.f, -10.f), Color(1.f, 1.f, 1.f));

    REQUIRE(w.ContainsLight(light));
}