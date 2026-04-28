#include "color.hpp"
#include "computations.hpp"
#include "intersection.hpp"
#include "maths.hpp"
#include "patterns/test_pattern.hpp"
#include "ray.hpp"
#include "renderer.hpp"
#include "shapes/group.hpp"
#include "shapes/plane.hpp"
#include "shapes/sphere.hpp"
#include "world.hpp"
#include <catch2/catch_test_macros.hpp>

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
    Shape& mutableS = world.GetMutableObject(id);
    mutableS.GetMutableMaterial().SetColor(Color(0.5f, 0.5f, 0.5f));
    const Shape& constS = world.GetObject(id);
    REQUIRE(constS.GetMaterial().GetColor() == Color(0.5f, 0.5f, 0.5f));
}

TEST_CASE("Test GetMutableLight changes persist in the world", "[world]")
{
    World world;
    Light l(Point(0.f, 0.f, 0.f), Color(1.f, 1.f, 1.f));
    world.AddLight(l);
    Light& mutableL = world.GetMutableLight(0);
    mutableL.SetPosition(Point(1.f, 1.f, 1.f));
    mutableL.SetIntensity(Color(0.5f, 0.5f, 0.5f));
    const Light& constL = world.GetLight(0);
    REQUIRE(constL.GetPosition() == Point(1.f, 1.f, 1.f));
    REQUIRE(constL.GetIntensity() == Color(0.5f, 0.5f, 0.5f));
}

TEST_CASE("Adding an object to the world returns a stable object ID", "[world]")
{
    World world;
    Sphere s("s");
    ObjectId id = world.AddObject(s);
    ObjectId expected = s.GetObjectId();
    REQUIRE(id == expected);
}

TEST_CASE("Getting an invalid object ID throws", "[world]")
{
    World world;
    REQUIRE_THROWS_AS(world.GetObject(kInvalidObjectId), std::invalid_argument);
    REQUIRE_THROWS_AS(world.GetObject(99), std::out_of_range);
}

TEST_CASE("Getting an invalid light index throws", "[world]")
{
    World world;
    REQUIRE_THROWS_AS(world.GetLight(0), std::out_of_range);
    REQUIRE_THROWS_AS(world.GetMutableLight(0), std::out_of_range);
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
    REQUIRE(w.GetLight(0).GetPosition() == Point(0.f, 0.25f, 0.f));
    REQUIRE(w.GetLight(0).GetIntensity() == Color(1.f, 1.f, 1.f));
}

TEST_CASE("Shading an intersection", "[world]")
{
    World w = Renderer::DefaultWorld();
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    ObjectId objectId = w.GetObjectWithName("external").GetObjectId();
    Intersection i(4.f, objectId);
    auto comps = Renderer::PrepareComputations(i, r, w);
    auto color = Renderer::ShadeHit(w, comps, Renderer::kMaxRecursionDepth);
    REQUIRE(color == Color(0.38066f, 0.47583f, 0.2855f));
}

TEST_CASE("Shading an intersection from the inside", "[world]")
{
    World w = Renderer::DefaultWorld();
    w.ReplaceLight(0, Light(Point(0.f, 0.25f, 0.f), Color(1.f, 1.f, 1.f)));
    Ray r(Point(0.f, 0.f, 0.f), Tuple(0.f, 0.f, 1.f));
    ObjectId objectId = w.GetObjectWithName("internal").GetObjectId();
    Intersection i(0.5f, objectId);
    auto comps = Renderer::PrepareComputations(i, r, w);
    auto color = Renderer::ShadeHit(w, comps, Renderer::kMaxRecursionDepth);
    REQUIRE(color == Color(0.90498f, 0.90498f, 0.90498f));
}

TEST_CASE("Shading an intersection accumulates contributions from multiple lights", "[world]")
{
    World world;
    Sphere sphere("sphere");
    ObjectId objectId = world.AddObject(sphere);
    world.AddLight(Light(Point(0.f, 0.f, -10.f), Color(1.f, 1.f, 1.f)));
    world.AddLight(Light(Point(0.f, 0.f, -10.f), Color(1.f, 1.f, 1.f)));

    Ray ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    Intersection intersection(4.f, objectId);
    Computations comps = Renderer::PrepareComputations(intersection, ray, world);

    REQUIRE(Renderer::ShadeHit(world, comps, Renderer::kMaxRecursionDepth) == Color(3.8f, 3.8f, 3.8f));
}

TEST_CASE("The colour when a ray misses", "[world]")
{
    World w = Renderer::DefaultWorld();
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 1.f, 0.f));
    Color color = Renderer::ColorAt(w, r);
    REQUIRE(color == Color(0.f, 1.f, 1.f));
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
    Shape& outer = w.GetMutableObjectWithName("external");
    Shape& inner = w.GetMutableObjectWithName("internal");
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

TEST_CASE("There is no shadow when nothing is collinear with point and light", "[world]")
{
    World w = Renderer::DefaultWorld();
    Tuple point = Point(0.f, 10.f, 0.f);
    REQUIRE(Renderer::IsShadowed(w, point) == EInShadow::No);
}

TEST_CASE("The shadow when an object is between the point and the light", "[world]")
{
    World w = Renderer::DefaultWorld();
    Tuple point = Point(10.f, -10.f, 10.f);
    REQUIRE(Renderer::IsShadowed(w, point) == EInShadow::Yes);
}

TEST_CASE("Shadows are evaluated per light", "[world]")
{
    World world;
    world.AddLight(Light(Point(0.f, 0.f, -10.f), Color(1.f, 1.f, 1.f)));
    world.AddLight(Light(Point(0.f, 10.f, -10.f), Color(1.f, 1.f, 1.f)));

    Sphere blocker("blocker");
    blocker.SetTransform(Matrix::CreateTranslation(0.f, 0.f, -5.f));
    world.AddObject(blocker);

    const Tuple point = Point(0.f, 0.f, 0.f);
    REQUIRE(Renderer::IsShadowed(world, point, world.GetLight(0)) == EInShadow::Yes);
    REQUIRE(Renderer::IsShadowed(world, point, world.GetLight(1)) == EInShadow::No);
}

TEST_CASE("There is no shadow when an object is behind the light", "[world]")
{
    World w = Renderer::DefaultWorld();
    Tuple point = Point(-20.f, 20.f, -20.f);
    REQUIRE(Renderer::IsShadowed(w, point) == EInShadow::No);
}

TEST_CASE("There is no shadow when an object is behind the point", "[world]")
{
    World w = Renderer::DefaultWorld();
    Tuple point = Point(-2.f, 2.f, -2.f);
    REQUIRE(Renderer::IsShadowed(w, point) == EInShadow::No);
}

TEST_CASE("ShadeHit is given an intersection in shadow", "[world]")
{
    World w;
    Light light(Point(0.f, 0.f, -10.f), Color(1.f, 1.f, 1.f));
    w.AddLight(light);
    Sphere s1("s1");
    ObjectId s1id = w.AddObject(s1);
    Sphere s2("s2");
    s2.SetTransform(Matrix::CreateTranslation(0.f, 0.f, 10.f));
    ObjectId s2id = w.AddObject(s2);
    Ray r(Point(0.f, 0.f, 5.f), Tuple(0.f, 0.f, 1.f));
    Intersection i(4.f, s1id);
    Computations comps = Renderer::PrepareComputations(i, r, w);
    Color color = Renderer::ShadeHit(w, comps, Renderer::kMaxRecursionDepth);
    REQUIRE(color == Color(0.1f, 0.1f, 0.1f));
}

TEST_CASE("Shapes added to a world cannot share the same name", "[world]")
{
    World w;
    Shape s1("shape");
    Shape s2("shape");
    w.AddObject(s1);
    REQUIRE_THROWS_AS(w.AddObject(s2), std::invalid_argument);
}

TEST_CASE("The reflected color for a nonreflective material", "[world]")
{
    World w = Renderer::DefaultWorld();
    Ray r(Point(0.f, 0.f, 0.f), Tuple(0.f, 0.f, 1.f));
    Shape& inner = w.GetMutableObjectWithName("internal");
    inner.GetMutableMaterial().SetAmbient(1.f);
    Intersection i(1.f, inner.GetObjectId());
    Computations comps = Renderer::PrepareComputations(i, r, w);
    Color result = Renderer::ReflectedColor(w, comps, Renderer::kMaxRecursionDepth);

    REQUIRE(result == kColorBlack);
}

TEST_CASE("The reflected color for a reflective material", "[world]")
{
    World w = Renderer::DefaultWorld();
    Plane shape("plane");
    shape.SetTransform(Matrix::CreateTranslation(0.f, -1.f, 0.f));
    shape.GetMutableMaterial().SetReflective(0.5f);
    w.AddObject(shape);

    Ray r(Point(0.f, 0.f, -3.f), Tuple(0.f, -std::sqrt(2.f) / 2.f, std::sqrt(2.f) / 2.f));
    Intersection i(std::sqrt(2.f), shape.GetObjectId());
    Computations comps = Renderer::PrepareComputations(i, r, w);
    Color result = Renderer::ReflectedColor(w, comps, Renderer::kMaxRecursionDepth);

    REQUIRE(result == Color(0.19032f, 0.2379f, 0.14274f));
}

TEST_CASE("ShadeHit() with a reflected material", "[world]")
{
    World w = Renderer::DefaultWorld();
    Plane shape("plane");
    shape.SetTransform(Matrix::CreateTranslation(0.f, -1.f, 0.f));
    shape.GetMutableMaterial().SetReflective(0.5f);
    w.AddObject(shape);

    Ray r(Point(0.f, 0.f, -3.f), Tuple(0.f, -std::sqrt(2.f) / 2.f, std::sqrt(2.f) / 2.f));
    Intersection i(std::sqrt(2.f), shape.GetObjectId());
    Computations comps = Renderer::PrepareComputations(i, r, w);
    Color result = Renderer::ShadeHit(w, comps, Renderer::kMaxRecursionDepth);

    REQUIRE(result == Color(0.87677f, 0.92436f, 0.82918f));
}

TEST_CASE("ColorAt() with mutually reflective surfaces", "[world]")
{
    World w;
    Light light(Point(0.f, 0.f, 0.f), Color(1.f, 1.f, 1.f));
    w.AddLight(light);

    Plane lower("lower");
    lower.SetTransform(Matrix::CreateTranslation(0.f, -1.f, 0.f));
    Material lowerMaterial;
    lowerMaterial.SetReflective(1.f);
    lower.SetMaterial(lowerMaterial);
    w.AddObject(lower);

    Plane upper("upper");
    upper.SetTransform(Matrix::CreateTranslation(0.f, 1.f, 0.f));
    Material upperMaterial;
    upperMaterial.SetReflective(1.f);
    upper.SetMaterial(upperMaterial);
    w.AddObject(upper);

    Ray r(Point(0.f, 0.f, 0.f), Tuple(0.f, 1.f, 0.f));
    Renderer::ColorAt(w, r);
}

TEST_CASE("The reflected color at the maximum recuresive depth", "[world]")
{
    World w = Renderer::DefaultWorld();
    Light light(Point(0.f, 0.f, 0.f), Color(1.f, 1.f, 1.f));
    w.AddLight(light);

    Plane p("plane");
    p.SetTransform(Matrix::CreateTranslation(0.f, -1.f, 0.f));
    Material material;
    material.SetReflective(0.5f);
    p.SetMaterial(material);
    w.AddObject(p);

    Ray r(Point(0.f, 0.f, -3.f), Tuple(0.f, -std::sqrt(2.f) / 2.f, std::sqrt(2.f) / 2.f));
    Intersection i(std::sqrt(2.f), p.GetObjectId());
    Computations comps = Renderer::PrepareComputations(i, r, w);
    Color c = Renderer::ReflectedColor(w, comps, 0);

    REQUIRE(c == Color(0.f, 0.f, 0.f));
}

TEST_CASE("The refracted color with an opaque surface", "[world]")
{
    World w = Renderer::DefaultWorld();
    Shape s = w.GetObjectWithName("external");
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    std::vector<Intersection> xs = {
        Intersection(4.f, s.GetObjectId()),
        Intersection(6.f, s.GetObjectId()),
    };
    Computations comps = Renderer::PrepareComputations(xs[0], r, w, &xs);
    Color c = Renderer::RefractedColor(w, comps, Renderer::kMaxRecursionDepth);

    REQUIRE(c == Color(0.f, 0.f, 0.f));
}

TEST_CASE("The refracted color at the maximum recursive depth is black", "[world]")
{
    World w = Renderer::DefaultWorld();
    Shape& s = w.GetMutableObjectWithName("external");
    s.GetMutableMaterial().SetTransparency(1.f);
    s.GetMutableMaterial().SetRefractiveIndex(1.5f);
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    std::vector<Intersection> xs = {
        Intersection(4.f, s.GetObjectId()),
        Intersection(6.f, s.GetObjectId()),
    };
    Computations comps = Renderer::PrepareComputations(xs[0], r, w, &xs);
    Color c = Renderer::RefractedColor(w, comps, 0);

    REQUIRE(c == kColorBlack);
}

TEST_CASE("The refracted color under total internal reflection", "[world]")
{
    World w = Renderer::DefaultWorld();
    Shape& s = w.GetMutableObjectWithName("external");
    s.GetMutableMaterial().SetTransparency(1.f);
    s.GetMutableMaterial().SetRefractiveIndex(1.5f);
    Ray r(Point(0.f, 0.f, std::sqrt(2.f) / 2.f), Tuple(0.f, 1.f, 0.f));
    std::vector<Intersection> xs = {
        Intersection(-std::sqrt(2.f) / 2.f, s.GetObjectId()),
        Intersection(std::sqrt(2.f) / 2.f, s.GetObjectId()),
    };
    Computations comps = Renderer::PrepareComputations(xs[1], r, w, &xs);
    Color c = Renderer::RefractedColor(w, comps, 5);

    REQUIRE(c == kColorBlack);
}

TEST_CASE("The refracted color with a refracted ray", "[world]")
{
    World w = Renderer::DefaultWorld();
    Shape& a = w.GetMutableObjectWithName("external");
    Material& materiala = a.GetMutableMaterial();
    materiala.SetAmbient(1.f);
    TestPattern pattern;
    materiala.SetPattern(pattern);

    Shape& b = w.GetMutableObjectWithName("internal");
    Material& materialb = b.GetMutableMaterial();
    materialb.SetTransparency(1.f);
    materialb.SetRefractiveIndex(1.5f);

    Ray r(Point(0.f, 0.f, 0.1f), Tuple(0.f, 1.f, 0.f));
    std::vector<Intersection> xs = {
        Intersection(-0.9899f, a.GetObjectId()),
        Intersection(-0.4899f, b.GetObjectId()),
        Intersection(0.4899f, b.GetObjectId()),
        Intersection(0.9899f, a.GetObjectId()),
    };
    Computations comps = Renderer::PrepareComputations(xs[2], r, w, &xs);
    Color c = Renderer::RefractedColor(w, comps, 5);

    REQUIRE(c == Color(0.f, 0.99888f, 0.04725f));
}

TEST_CASE("ShadeHit() with a transparent material", "[world]")
{
    World w = Renderer::DefaultWorld();
    Plane floor("floor");
    floor.SetTransform(Matrix::CreateTranslation(0.f, -1.f, 0.f));
    floor.GetMutableMaterial().SetTransparency(0.5f);
    floor.GetMutableMaterial().SetRefractiveIndex(2.5f);
    w.AddObject(floor);

    Sphere ball("ball");
    ball.SetTransform(Matrix::CreateTranslation(0.f, -3.5f, -0.5f));
    ball.GetMutableMaterial().SetColor(Color(1.f, 0.f, 0.f));
    ball.GetMutableMaterial().SetAmbient(0.5f);
    w.AddObject(ball);

    Ray r(Point(0.f, 0.f, -3.f), Tuple(0.f, -std::sqrt(2.f) / 2.f, std::sqrt(2.f) / 2.f));
    std::vector<Intersection> xs = {
        Intersection(std::sqrt(2.f), floor.GetObjectId()),
    };
    Computations comps = Renderer::PrepareComputations(xs[0], r, w, &xs);
    Color c = Renderer::ShadeHit(w, comps, 5);

    REQUIRE(c == Color(0.93642f, 0.68642f, 0.68642f));
}

TEST_CASE("ShadeHit() with a reflective, transparent material", "[world]")
{
    World w = Renderer::DefaultWorld();
    Ray r(Point(0.f, 0.f, -3.f), Tuple(0.f, -std::sqrt(2.f) / 2.f, std::sqrt(2.f) / 2.f));

    Plane floor("floor");
    floor.SetTransform(Matrix::CreateTranslation(0.f, -1.f, 0.f));
    Material floorMaterial;
    floorMaterial.SetReflective(0.5f);
    floorMaterial.SetTransparency(0.5f);
    floorMaterial.SetRefractiveIndex(1.5f);
    floor.SetMaterial(floorMaterial);
    w.AddObject(floor);

    Sphere ball = Sphere("ball");
    Material ballMaterial;
    ballMaterial.SetColor(Color(1.f, 0.f, 0.f));
    ballMaterial.SetAmbient(0.5f);
    ball.SetMaterial(ballMaterial);
    ball.SetTransform(Matrix::CreateTranslation(0.f, -3.5f, -0.5f));
    w.AddObject(ball);

    std::vector<Intersection> xs = {
        Intersection(std::sqrt(2.f), floor.GetObjectId()),
    };
    Computations comps = Renderer::PrepareComputations(xs[0], r, w, &xs);
    Color c = Renderer::ShadeHit(w, comps, 5);

    REQUIRE(c == Color(0.93391f, 0.69643f, 0.69243f));
}

TEST_CASE("A group copied into the world should not alias the original child shape", "[world][groups][ownership]")
{
    World world;
    Group group("group");
    Sphere child("child");
    child.SetTransform(Matrix::CreateTranslation(0.f, 0.f, -3.f));
    group.AddChild(child);
    world.AddObject(group);

    const Ray ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    const std::vector<Intersection> beforeMutation = Renderer::IntersectWorld(world, ray);
    REQUIRE(beforeMutation.size() == 2);

    child.SetTransform(Matrix::CreateTranslation(5.f, 0.f, 0.f));

    const std::vector<Intersection> afterMutation = Renderer::IntersectWorld(world, ray);
    REQUIRE(afterMutation.size() == 2);
    REQUIRE(AreEqual(afterMutation[0].GetT(), beforeMutation[0].GetT()));
    REQUIRE(AreEqual(afterMutation[1].GetT(), beforeMutation[1].GetT()));
}

TEST_CASE("Intersecting a world-owned group should preserve the child object ID", "[world][groups][ownership]")
{
    World world;
    Group group("group");
    Sphere child("child");
    child.SetTransform(Matrix::CreateTranslation(0.f, 0.f, -3.f));
    group.AddChild(child);
    const ObjectId groupId = world.AddObject(group);
    const ObjectId childId = world.GetObjectWithName("child").GetObjectId();

    const Ray ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    const std::vector<Intersection> xs = Renderer::IntersectWorld(world, ray);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].GetObjectId() == childId);
    REQUIRE(xs[1].GetObjectId() == childId);
    REQUIRE(xs[0].GetObjectId() != groupId);
}

TEST_CASE("Shading a grouped child should use the child's material", "[world][groups][ownership]")
{
    World world;
    world.AddLight(Light(Point(0.f, 0.f, -10.f), Color(1.f, 1.f, 1.f)));

    Group group("group");
    group.GetMutableMaterial().SetColor(kColorGreen).SetAmbient(1.f).SetDiffuse(0.f).SetSpecular(0.f);

    Sphere child("child");
    child.GetMutableMaterial().SetColor(kColorRed).SetAmbient(1.f).SetDiffuse(0.f).SetSpecular(0.f);
    child.SetTransform(Matrix::CreateTranslation(0.f, 0.f, -3.f));
    group.AddChild(child);
    world.AddObject(group);

    const Color color = Renderer::ColorAt(world, Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f)));
    REQUIRE(color == kColorRed);
}
