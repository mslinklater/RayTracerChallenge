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

TEST_CASE("Test ContainsObject and ContainsLight", "[world]")
{
    World world;
    Sphere s("s");
    Light l(Point(0.f, 0.f, 0.f), Color(1.f, 1.f, 1.f));
    REQUIRE_FALSE(world.ContainsObject(s));
    REQUIRE_FALSE(world.ContainsLight(l));
    world.AddObject(s);
    world.AddLight(l);
    REQUIRE(world.ContainsObject(s));
    REQUIRE(world.ContainsLight(l));
}

TEST_CASE("Test GetMutableObject changes persist in the world", "[world]")
{
    World world;
    Sphere s("s");
    ObjectId id = world.AddObject(s);
    Sphere &mutableS = world.GetMutableObject(id);
    mutableS.GetMutableMaterial().SetColor(Color(0.5f, 0.5f, 0.5f));
    const Sphere &constS = world.GetObject(id);
    REQUIRE(constS.GetMaterial().GetColor() == Color(0.5f, 0.5f, 0.5f));
}

TEST_CASE("Test GetMutableLight changes persist in the world", "[world]")
{
    World world;
    Light l(Point(0.f, 0.f, 0.f), Color(1.f, 1.f, 1.f));
    world.AddLight(l);
    Light &mutableL = world.GetMutableLight(0);
    mutableL.position = Point(1.f, 1.f, 1.f);
    mutableL.intensity = Color(0.5f, 0.5f, 0.5f);
    const Light &constL = world.GetLight(0);
    REQUIRE(constL.position == Point(1.f, 1.f, 1.f));
    REQUIRE(constL.intensity == Color(0.5f, 0.5f, 0.5f));
}

TEST_CASE("Adding an object to the world returns a stable object ID", "[world]")
{
    World world;
    Sphere s("s");
    ObjectId id = world.AddObject(s);
    REQUIRE(id == 0);
}

TEST_CASE("The default world", "[world]")
{
    World w = Renderer::DefaultWorld();

    Light light = Light(Point(-10.f, 10.f, -10.f), Color(1.f, 1.f, 1.f));
    w.AddLight(light);

    Sphere s1("s1");
    s1.GetMutableMaterial().SetColor(Color(0.8f, 1.f, 0.6f));
    s1.GetMutableMaterial().SetDiffuse(0.7f);
    s1.GetMutableMaterial().SetSpecular(0.2f);

    Sphere s2("s2");
    s2.SetTransform(Matrix::CreateScaling(0.5f, 0.5f, 0.5f));

    REQUIRE(w.ContainsObject(s1));
    REQUIRE(w.ContainsObject(s2));
    REQUIRE(w.ContainsLight(light));
}

TEST_CASE("Intersect a world with a ray", "[world]")
{
    World w = Renderer::DefaultWorld();
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    auto xs = Renderer::IntersectWorld(w, r);
    REQUIRE(xs.size() == 4);
    REQUIRE(AreEqual(xs[0].GetT(), 4.f));
    REQUIRE(AreEqual(xs[1].GetT(), 4.5f));
    REQUIRE(AreEqual(xs[2].GetT(), 5.5f));
    REQUIRE(AreEqual(xs[3].GetT(), 6.f));
}

TEST_CASE("Is able to replace a light in the world", "[world]")
{
    World w = Renderer::DefaultWorld();
    Light light(Point(0.f, 0.25f, 0.f), Color(1.f, 1.f, 1.f));
    w.AddLight(light);
    Light newLight(Point(0.f, 0.25f, 0.f), Color(1.f, 1.f, 1.f));
    w.ReplaceLight(0, newLight);
    REQUIRE(w.GetLight(0).position == Point(0.f, 0.25f, 0.f));
    REQUIRE(w.GetLight(0).intensity == Color(1.f, 1.f, 1.f));
}

TEST_CASE("Shading an intersection", "[world]")
{
    World w = Renderer::DefaultWorld();
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    Intersection i(4.f, 0);
    auto comps = Renderer::PrepareComputations(i, r, w);
    auto color = Renderer::ShadeHit(w, comps);
    REQUIRE(color == Color(0.38066f, 0.47583f, 0.2855f));
}

TEST_CASE("Shading an intersection from the inside", "[world]")
{
    World w = Renderer::DefaultWorld();
    w.ReplaceLight(0, Light(Point(0.f, 0.25f, 0.f), Color(1.f, 1.f, 1.f)));
    Ray r(Point(0.f, 0.f, 0.f), Tuple(0.f, 0.f, 1.f));
    Intersection i(0.5f, 1);
    auto comps = Renderer::PrepareComputations(i, r, w);
    auto color = Renderer::ShadeHit(w, comps);
    REQUIRE(color == Color(0.90498f, 0.90498f, 0.90498f));
}

TEST_CASE("The colour when a ray misses", "[world]")
{
    World w = Renderer::DefaultWorld();
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 1.f, 0.f));
    Color color = Renderer::ColorAt(w, r);
    REQUIRE(color == Color(0.f, 0.f, 0.f));
}

TEST_CASE("The colour when a ray hits", "[world]")
{
    World w = Renderer::DefaultWorld();
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    Color color = Renderer::ColorAt(w, r);
    REQUIRE(color == Color(0.38066f, 0.47583f, 0.2855f));
}

TEST_CASE("The colour with an intersection behind the ray", "[world]")
{
    World w = Renderer::DefaultWorld();
    Sphere &outer = w.GetMutableObject(0);
    Sphere &inner = w.GetMutableObject(1);
    outer.GetMutableMaterial().SetAmbient(1.f);
    inner.GetMutableMaterial().SetAmbient(1.f);
    Ray r(Point(0.f, 0.f, 0.75f), Tuple(0.f, 0.f, -1.f));
    Color color = Renderer::ColorAt(w, r);
    REQUIRE(color == inner.GetMaterial().GetColor());
}

TEST_CASE("World with one spheres to the side. Make sure sphere is able to be hit", "[world]")
{
    World w;
    Sphere s1("s1");
    s1.SetTransform(Matrix::CreateTranslation(2.f, 5.f, 0.f));
    ObjectId worldSphereId = w.AddObject(s1);
    Ray r1(Point(2.f, 5.f, -5.f), Tuple(0.f, 0.f, 1.f));
    auto xs1 = Renderer::IntersectWorld(w, r1);
    REQUIRE(xs1.size() == 2);
    REQUIRE(xs1[0].GetObjectId() == worldSphereId);
    REQUIRE(xs1[1].GetObjectId() == worldSphereId);
}

TEST_CASE("World with two spheres side by side. Make sure each sphere is able to be hit", "[world]")
{
    World w;
    Sphere s1("s1");
    Sphere s2("s2");
    s1.SetTransform(Matrix::CreateTranslation(2.f, 0.f, 0.f));
    s2.SetTransform(Matrix::CreateTranslation(-2.f, 0.f, 0.f));
    ObjectId worldS1Id = w.AddObject(s1);
    ObjectId worldS2Id = w.AddObject(s2);
    Ray r1(Point(2.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    Ray r2(Point(-2.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    std::vector<Intersection> xs1 = Renderer::IntersectWorld(w, r1);
    std::vector<Intersection> xs2 = Renderer::IntersectWorld(w, r2);
    REQUIRE(xs1.size() == 2);
    REQUIRE(xs1[0].GetObjectId() == worldS1Id);
    REQUIRE(xs1[1].GetObjectId() == worldS1Id);
    REQUIRE(xs2.size() == 2);
    REQUIRE(xs2[0].GetObjectId() == worldS2Id);
    REQUIRE(xs2[1].GetObjectId() == worldS2Id);
}

TEST_CASE("World with two spheres side by side. Make sure the object hit has a known ID", "[world]")
{
    World w;
    Sphere s1("s1");
    Sphere s2("s2");
    s1.SetTransform(Matrix::CreateTranslation(2.f, 0.f, 0.f));
    s2.SetTransform(Matrix::CreateTranslation(-2.f, 0.f, 0.f));
    ObjectId worldS1Id = w.AddObject(s1);
    ObjectId worldS2Id = w.AddObject(s2);
    Ray r1(Point(2.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    std::vector<Intersection> xs1 = Renderer::IntersectWorld(w, r1);
    REQUIRE(xs1.size() == 2);
    bool bOneOfThemHit = (xs1[0].GetObjectId() == worldS1Id) || (xs1[0].GetObjectId() == worldS2Id);
    REQUIRE(bOneOfThemHit);
}
