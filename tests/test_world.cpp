#include <catch2/catch_test_macros.hpp>
#include "world.hpp"
#include "ray.hpp"
#include "intersection.hpp"
#include "maths.hpp"
#include "renderer.hpp"
#include "computations.hpp"

TEST_CASE("Creating a world", "[world]")
{
    World world;
    REQUIRE(world.GetObjects().empty());
    REQUIRE(world.GetLights().empty());
}

TEST_CASE("Adding an object to the world returns a ptr to the stored object, not the one passed in", "[world]")
{
    World world;
    Sphere s;
    const Sphere *storedSphere = world.AddObject(s);
    REQUIRE(storedSphere != &s);
}

TEST_CASE("The default world", "[world]")
{
    World w = DefaultWorld();

    Light light = Light(Point(-10.f, 10.f, -10.f), Color(1.f, 1.f, 1.f));
    w.AddLight(light);

    Sphere s1;
    s1.GetMutableMaterial().SetColor(Color(0.8f, 1.f, 0.6f));
    s1.GetMutableMaterial().SetDiffuse(0.7f);
    s1.GetMutableMaterial().SetSpecular(0.2f);

    Sphere s2;
    s2.SetTransform(Matrix::CreateScaling(0.5f, 0.5f, 0.5f));

    REQUIRE(w.ContainsObject(s1));
    REQUIRE(w.ContainsObject(s2));
}

TEST_CASE("Intersect a world with a ray", "[world]")
{
    World w = DefaultWorld();
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    auto xs = IntersectWorld(w, r);
    REQUIRE(xs.size() == 4);
    REQUIRE(AreEqual(xs[0].GetT(), 4.f));
    REQUIRE(AreEqual(xs[1].GetT(), 4.5f));
    REQUIRE(AreEqual(xs[2].GetT(), 5.5f));
    REQUIRE(AreEqual(xs[3].GetT(), 6.f));
}

TEST_CASE("Is able to replace a light in the world", "[world]")
{
    World w = DefaultWorld();
    Light light(Point(0.f, 0.25f, 0.f), Color(1.f, 1.f, 1.f));
    w.AddLight(light);
    Light newLight(Point(0.f, 0.25f, 0.f), Color(1.f, 1.f, 1.f));
    w.ReplaceLight(0, newLight);
    REQUIRE(w.GetLight(0).position == Point(0.f, 0.25f, 0.f));
    REQUIRE(w.GetLight(0).intensity == Color(1.f, 1.f, 1.f));
}

TEST_CASE("Shading an intersection", "[world]")
{
    World w = DefaultWorld();
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    Sphere shape = w.GetObject(0);
    Intersection i(4.f, &shape);
    auto comps = PrepareComputations(i, r);
    auto color = ShadeHit(w, comps);
    REQUIRE(color == Color(0.38066f, 0.47583f, 0.2855f));
}

TEST_CASE("Shading an intersection from the inside", "[world]")
{
    World w = DefaultWorld();
    w.ReplaceLight(0, Light(Point(0.f, 0.25f, 0.f), Color(1.f, 1.f, 1.f)));
    Ray r(Point(0.f, 0.f, 0.f), Tuple(0.f, 0.f, 1.f));
    Sphere shape = w.GetObject(1);
    Intersection i(0.5f, &shape);
    auto comps = PrepareComputations(i, r);
    auto color = ShadeHit(w, comps);
    REQUIRE(color == Color(0.90498f, 0.90498f, 0.90498f));
}

TEST_CASE("The colour when a ray misses", "[world]")
{
    World w = DefaultWorld();
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 1.f, 0.f));
    Color color = ColorAt(w, r);
    REQUIRE(color == Color(0.f, 0.f, 0.f));
}

TEST_CASE("The colour when a ray hits", "[world]")
{
    World w = DefaultWorld();
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    Color color = ColorAt(w, r);
    REQUIRE(color == Color(0.38066f, 0.47583f, 0.2855f));
}

TEST_CASE("The colour with an intersection behind the ray", "[world]")
{
    World w = DefaultWorld();
    Sphere &outer = w.GetMutableObject(0);
    Sphere &inner = w.GetMutableObject(1);
    outer.GetMutableMaterial().SetAmbient(1.f);
    inner.GetMutableMaterial().SetAmbient(1.f);
    Ray r(Point(0.f, 0.f, 0.75f), Tuple(0.f, 0.f, -1.f));
    Color color = ColorAt(w, r);
    REQUIRE(color == inner.GetMaterial().GetColor());
}

TEST_CASE("World with one spheres to the side. Make sure sphere is able to be hit", "[world]")
{
    World w;
    Sphere s1;
    s1.SetTransform(Matrix::CreateTranslation(2.f, 5.f, 0.f));
    const Sphere *worldSphere = w.AddObject(s1);
    Ray r1(Point(2.f, 5.f, -5.f), Tuple(0.f, 0.f, 1.f));
    auto xs1 = IntersectWorld(w, r1);
    REQUIRE(xs1.size() == 2);
    REQUIRE(xs1[0].GetObject() == worldSphere);
    REQUIRE(xs1[1].GetObject() == worldSphere);
}

TEST_CASE("World with two spheres side by side. Make sure each sphere is able to be hit", "[world]")
{
    World w;
    Sphere s1;
    Sphere s2;
    s1.SetTransform(Matrix::CreateTranslation(2.f, 0.f, 0.f));
    s2.SetTransform(Matrix::CreateTranslation(-2.f, 0.f, 0.f));
    const Sphere *worldS1 = w.AddObject(s1);
    const Sphere *worldS2 = w.AddObject(s2);
    Ray r1(Point(2.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    Ray r2(Point(-2.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    auto xs1 = IntersectWorld(w, r1);
    auto xs2 = IntersectWorld(w, r2);
    REQUIRE(xs1.size() == 2);
    REQUIRE(xs1[0].GetObject() == worldS1);
    REQUIRE(xs1[1].GetObject() == worldS1);
    REQUIRE(xs2.size() == 2);
    REQUIRE(xs2[0].GetObject() == worldS2);
    REQUIRE(xs2[1].GetObject() == worldS2);
}