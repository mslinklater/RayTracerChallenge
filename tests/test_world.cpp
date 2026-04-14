#include "color.hpp"
#include "computations.hpp"
#include "intersection.hpp"
#include "maths.hpp"
#include "patterns/test_pattern.hpp"
#include "ray.hpp"
#include "renderer.hpp"
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
    Shape &mutableS = world.GetMutableObject(id);
    mutableS.GetMutableMaterial().SetColor(Color(0.5f, 0.5f, 0.5f));
    const Shape &constS = world.GetObject(id);
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
    ObjectId expected = s.GetWorldObjectId();
    REQUIRE(id == expected);
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
    ObjectId objectId = w.GetObjectWithName("external").GetWorldObjectId();
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
    ObjectId objectId = w.GetObjectWithName("internal").GetWorldObjectId();
    Intersection i(0.5f, objectId);
    auto comps = Renderer::PrepareComputations(i, r, w);
    auto color = Renderer::ShadeHit(w, comps, Renderer::kMaxRecursionDepth);
    REQUIRE(color == Color(0.90498f, 0.90498f, 0.90498f));
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
    Shape &outer = w.GetMutableObjectWithName("external");
    Shape &inner = w.GetMutableObjectWithName("internal");
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
    Shape &inner = w.GetMutableObjectWithName("internal");
    inner.GetMutableMaterial().SetAmbient(1.f);
    Intersection i(1.f, inner.GetWorldObjectId());
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
    Intersection i(std::sqrt(2.f), shape.GetWorldObjectId());
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
    Intersection i(std::sqrt(2.f), shape.GetWorldObjectId());
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
    Intersection i(std::sqrt(2.f), p.GetWorldObjectId());
    Computations comps = Renderer::PrepareComputations(i, r, w);
    Color c = Renderer::ReflectedColor(w, comps, 0);

    REQUIRE(c == Color(0.f, 0.f, 0.f));
}

TEST_CASE("The refracted color with an opaque surface", "[world]")
{
    World w = Renderer::DefaultWorld();
    Shape s = w.GetObjectWithName("external");
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    IntersectionVector xs = {
        Intersection(4.f, s.GetWorldObjectId()),
        Intersection(6.f, s.GetWorldObjectId()),
    };
    Computations comps = Renderer::PrepareComputations(xs[0], r, w, &xs);
    Color c = Renderer::RefractedColor(w, comps, Renderer::kMaxRecursionDepth);

    REQUIRE(c == Color(0.f, 0.f, 0.f));
}

TEST_CASE("The refracted color at the maximum recursive depth is black", "[world]")
{
    World w = Renderer::DefaultWorld();
    Shape &s = w.GetMutableObjectWithName("external");
    s.GetMutableMaterial().SetTransparency(1.f);
    s.GetMutableMaterial().SetRefractiveIndex(1.5f);
    Ray r(Point(0.f, 0.f, -5.f), Tuple(0.f, 0.f, 1.f));
    IntersectionVector xs = {
        Intersection(4.f, s.GetWorldObjectId()),
        Intersection(6.f, s.GetWorldObjectId()),
    };
    Computations comps = Renderer::PrepareComputations(xs[0], r, w, &xs);
    Color c = Renderer::RefractedColor(w, comps, 0);

    REQUIRE(c == kColorBlack);
}

TEST_CASE("The refracted color under total internal reflection", "[world]")
{
    World w = Renderer::DefaultWorld();
    Shape &s = w.GetMutableObjectWithName("external");
    s.GetMutableMaterial().SetTransparency(1.f);
    s.GetMutableMaterial().SetRefractiveIndex(1.5f);
    Ray r(Point(0.f, 0.f, std::sqrt(2.f) / 2.f), Tuple(0.f, 1.f, 0.f));
    IntersectionVector xs = {
        Intersection(-std::sqrt(2.f) / 2.f, s.GetWorldObjectId()),
        Intersection(std::sqrt(2.f) / 2.f, s.GetWorldObjectId()),
    };
    Computations comps = Renderer::PrepareComputations(xs[1], r, w, &xs);
    Color c = Renderer::RefractedColor(w, comps, 5);

    REQUIRE(c == kColorBlack);
}

TEST_CASE("The refracted color with a refracted ray", "[world]")
{
    World w = Renderer::DefaultWorld();
    Shape &a = w.GetMutableObjectWithName("external");
    Material &materiala = a.GetMutableMaterial();
    materiala.SetAmbient(1.f);
    TestPattern pattern;
    materiala.SetPattern(pattern);

    Shape &b = w.GetMutableObjectWithName("internal");
    Material &materialb = b.GetMutableMaterial();
    materialb.SetTransparency(1.f);
    materialb.SetRefractiveIndex(1.5f);

    Ray r(Point(0.f, 0.f, 0.1f), Tuple(0.f, 1.f, 0.f));
    IntersectionVector xs = {
        Intersection(-0.9899f, a.GetWorldObjectId()),
        Intersection(-0.4899f, b.GetWorldObjectId()),
        Intersection(0.4899f, b.GetWorldObjectId()),
        Intersection(0.9899f, a.GetWorldObjectId()),
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
    IntersectionVector xs = {
        Intersection(std::sqrt(2.f), floor.GetWorldObjectId()),
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

    IntersectionVector xs = {
        Intersection(std::sqrt(2.f), floor.GetWorldObjectId()),
    };
    Computations comps = Renderer::PrepareComputations(xs[0], r, w, &xs);
    Color c = Renderer::ShadeHit(w, comps, 5);

    REQUIRE(c == Color(0.93391f, 0.69643f, 0.69243f));
}
