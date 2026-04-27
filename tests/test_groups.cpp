#include "intersection.hpp"
#include "maths.hpp"
#include "matrix.hpp"
#include "shapes/group.hpp"
#include "shapes/sphere.hpp"
#include "test_shapes.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Creating a new group", "[groups]")
{
    Group g("group");
    Matrix identity(4);
    identity.SetIdentity();
    REQUIRE(g.GetTransform() == identity);
    REQUIRE(g.GetNumChildren() == 0);
}

TEST_CASE("A shape has a parent attribute", "[groups]")
{
    TestShape s("shape");
    REQUIRE(s.GetParent() == nullptr);
}

TEST_CASE("Adding a child to a group", "[groups]")
{
    Group g("group");
    TestShape s("shape");
    TestShape& stored = g.AddChild(s);
    REQUIRE(g.GetNumChildren() == 1);
    REQUIRE(g.Contains(s));
    REQUIRE(stored.GetParent() == &g);
    REQUIRE(&stored != &s);
}

TEST_CASE("Adding two children to a group", "[groups]")
{
    Group g("group");
    TestShape s1("shape1");
    TestShape s2("shape2");
    TestShape& stored1 = g.AddChild(s1);
    TestShape& stored2 = g.AddChild(s2);
    REQUIRE(g.GetNumChildren() == 2);
    REQUIRE(g.Contains(s1));
    REQUIRE(g.Contains(s2));
    REQUIRE(stored1.GetParent() == &g);
    REQUIRE(stored2.GetParent() == &g);
}

TEST_CASE("Adding a group as a child of a group", "[groups]")
{
    Group g1("group1");
    Group g2("group2");
    Group& stored = g1.AddChild(g2);
    REQUIRE(g1.GetNumChildren() == 1);
    REQUIRE(stored.GetNumChildren() == 0);
    REQUIRE(g1.Contains(g2));
    REQUIRE(stored.GetParent() == &g1);
}

TEST_CASE("Adding a group to itself throws", "[groups]")
{
    Group g("group");
    REQUIRE_THROWS_AS(g.AddChild(g), std::invalid_argument);
}

TEST_CASE("Adding a child with a duplicate name throws", "[groups]")
{
    Group g("group");
    TestShape s1("shape");
    TestShape s2("shape");
    g.AddChild(s1);

    REQUIRE_THROWS_AS(g.AddChild(s2), std::invalid_argument);
}

TEST_CASE("Intersecting a ray with an empty group", "[groups]")
{
    Group g("group");
    Ray r(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f));
    auto xs = g.IntersectLocal(r);
    REQUIRE(xs.empty());
}

TEST_CASE("Intersecting a ray with a non-empty group", "[groups]")
{
    Group g("group");
    Sphere s1("sphere1");
    Sphere s2("sphere2");
    s2.SetTransform(Matrix::CreateTranslation(0.f, 0.f, -3.f));
    Sphere s3("sphere3");
    s3.SetTransform(Matrix::CreateTranslation(5.f, 0.f, 0.f));
    Sphere& stored1 = g.AddChild(s1);
    Sphere& stored2 = g.AddChild(s2);
    g.AddChild(s3);
    stored1.SetObjectId(1);
    stored2.SetObjectId(2);
    Ray r(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    IntersectionVector xs = g.IntersectLocal(r);
    REQUIRE(xs.size() == 4);
    REQUIRE(xs[0].GetObjectId() == stored2.GetObjectId());
    REQUIRE(xs[1].GetObjectId() == stored2.GetObjectId());
    REQUIRE(xs[2].GetObjectId() == stored1.GetObjectId());
    REQUIRE(xs[3].GetObjectId() == stored1.GetObjectId());
}

TEST_CASE("Intersecting a transformed group", "[groups]")
{
    Group g("group");
    g.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    Sphere s("sphere");
    s.SetTransform(Matrix::CreateTranslation(5.f, 0.f, 0.f));
    Sphere& stored = g.AddChild(s);
    stored.SetObjectId(1);
    Ray r(Point(10.f, 0.f, -10.f), Vector(0.f, 0.f, 1.f));
    IntersectionVector xs = g.Intersect(r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].GetObjectId() == stored.GetObjectId());
    REQUIRE(xs[1].GetObjectId() == stored.GetObjectId());
}

TEST_CASE("Convert a point from world to object space", "[groups]")
{
    Group g1("group1");
    g1.SetTransform(Matrix::CreateRotationY(kPi / 2.f));
    Group g2("group2");
    g2.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    Group& storedG2 = g1.AddChild(g2);
    Sphere s("sphere");
    s.SetTransform(Matrix::CreateTranslation(5.f, 0.f, 0.f));
    Sphere& storedS = storedG2.AddChild(s);
    Tuple p = storedS.WorldToObject(Point(-2.f, 0.f, -10.f));
    REQUIRE(p == Point(0.f, 0.f, -1.f));
}

TEST_CASE("Converting a normal from object to world space", "[groups]")
{
    Group g1("group1");
    g1.SetTransform(Matrix::CreateRotationY(kPi / 2.f));
    Group g2("group2");
    g2.SetTransform(Matrix::CreateScaling(1.f, 2.f, 3.f));
    Group& storedG2 = g1.AddChild(g2);
    Sphere s("sphere");
    s.SetTransform(Matrix::CreateTranslation(5.f, 0.f, 0.f));
    Sphere& storedS = storedG2.AddChild(s);
    Tuple n = storedS.NormalToWorld(Vector(std::sqrt(3.f) / 3.f, std::sqrt(3.f) / 3.f, std::sqrt(3.f) / 3.f));
    REQUIRE(n == Vector(0.2857f, 0.4286f, -0.8571f));
}

#if 0
TEST_CASE("Finding the normal on a child object", "[groups]")
{
    Group g1("group1");
    g1.SetTransform(Matrix::CreateRotationY(kPi / 2.f));
    Group g2("group2");
    g2.SetTransform(Matrix::CreateScaling(1.f, 2.f, 3.f));
    g1.AddChild(&g2);
    Sphere s("sphere");
    s.SetTransform(Matrix::CreateTranslation(5.f, 0.f, 0.f));
    g2.AddChild(&s);
    Tuple n = s.NormalAt(Point(1.7321f, 1.1547f, -5.5774f));
    REQUIRE(n == Vector(0.2857f, 0.4286f, -0.8571f));
}

TEST_CASE("Moving a child between two groups", "[groups]")
{
    Group g1("group1");
    Group g2("group2");
    TestShape s("shape");
    g1.AddChild(&s);
    REQUIRE(g1.Contains(&s));
    g1.RemoveChild(&s);
    REQUIRE_FALSE(g1.Contains(&s));
    g2.AddChild(&s);
    REQUIRE(g2.Contains(&s));
}
#endif
