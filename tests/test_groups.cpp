#include "intersection.hpp"
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
    g.AddChild(&s);
    REQUIRE(g.GetNumChildren() == 1);
    REQUIRE(g.Contains(&s));
    REQUIRE(s.GetParent() == &g);
}

TEST_CASE("Adding two children to a group", "[groups]")
{
    Group g("group");
    TestShape s1("shape1");
    TestShape s2("shape2");
    g.AddChild(&s1);
    g.AddChild(&s2);
    REQUIRE(g.GetNumChildren() == 2);
    REQUIRE(g.Contains(&s1));
    REQUIRE(g.Contains(&s2));
    REQUIRE(s1.GetParent() == &g);
    REQUIRE(s2.GetParent() == &g);
}

TEST_CASE("Adding a group as a child of a group", "[groups]")
{
    Group g1("group1");
    Group g2("group2");
    g1.AddChild(&g2);
    REQUIRE(g1.GetNumChildren() == 1);
    REQUIRE(g2.GetNumChildren() == 0);
    REQUIRE(g1.Contains(&g2));
    REQUIRE(g2.GetParent() == &g1);
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
    g.AddChild(&s1);
    g.AddChild(&s2);
    g.AddChild(&s3);
    Ray r(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    IntersectionVector xs = g.IntersectLocal(r);
    REQUIRE(xs.size() == 4);
    REQUIRE(xs[0].GetObjectId() == s2.GetObjectId());
    REQUIRE(xs[1].GetObjectId() == s2.GetObjectId());
    REQUIRE(xs[2].GetObjectId() == s1.GetObjectId());
    REQUIRE(xs[3].GetObjectId() == s1.GetObjectId());
}

TEST_CASE("Intersecting a transformed group", "[groups]")
{
    Group g("group");
    g.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    Sphere s("sphere");
    s.SetTransform(Matrix::CreateTranslation(5.f, 0.f, 0.f));
    g.AddChild(&s);
    Ray r(Point(10.f, 0.f, -10.f), Vector(0.f, 0.f, 1.f));
    IntersectionVector xs = g.Intersect(r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].GetObjectId() == s.GetObjectId());
    REQUIRE(xs[1].GetObjectId() == s.GetObjectId());
}

#if 0
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
