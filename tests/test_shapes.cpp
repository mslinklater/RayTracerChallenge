#include "test_shapes.hpp"
#include "matrix.hpp"
#include "shapes/sphere.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("The default transformation", "[shapes]")
{
    TestShape s("shape");
    Matrix expected(4);
    expected.SetIdentity();
    REQUIRE(s.GetTransform() == expected);
}

TEST_CASE("Assigning a transformation", "[shapes]")
{
    TestShape s("shape");
    Matrix t = Matrix::CreateTranslation(2.f, 3.f, 4.f);
    s.SetTransform(t);
    Matrix expected(4);
    expected = Matrix::CreateTranslation(2.f, 3.f, 4.f);
    REQUIRE(s.GetTransform() == expected);
}

TEST_CASE("The default material", "[shapes]")
{
    TestShape s("sphere");
    Material expected;
    REQUIRE(s.GetMaterial() == expected);
}

TEST_CASE("Assigning a material", "[shapes]")
{
    TestShape s("sphere");
    Material m;
    m.SetAmbient(1.f);
    s.SetMaterial(m);
    Material expected;
    expected.SetAmbient(1.f);
    REQUIRE(s.GetMaterial() == expected);
}

TEST_CASE("Querying a shapes bounding box in its parents space", "[shapes]")
{
    Sphere s("sphere");
    s.SetTransform(Matrix::CreateTranslation(1.0f, -3.0f, 5.0f) * Matrix::CreateScaling(0.5f, 2.0f, 4.0f));
    BoundingBox b = s.GetBoundsInParentSpace();
    REQUIRE(b.GetMin() == Point(0.5f, -5.f, 1.f));
    REQUIRE(b.GetMax() == Point(1.5f, -1.f, 9.f));
}
