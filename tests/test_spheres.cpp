#include "material.hpp"
#include "maths.hpp"
#include "matrix.hpp"
#include "ray.hpp"
#include "renderer.hpp"
#include "shape.hpp"
#include "sphere.hpp"
#include "utils.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("A ray intersects a sphere at two points", "[spheres]")
{
    Sphere sphere("sphere");
    Ray ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    auto xs = sphere.Intersect(ray);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0] == 4.f);
    REQUIRE(xs[1] == 6.f);
}

TEST_CASE("Test Sphere equality operator", "[spheres]")
{
    Sphere s1("s1");
    Sphere s2("s1");
    Sphere s3("s3");

    REQUIRE(s1 == s2);
    REQUIRE(s1 != s3);
}

TEST_CASE("A ray intersects a sphere at tangent - two identical points", "[spheres]")
{
    Sphere sphere("sphere");
    Ray ray(Point(0.f, 1.f, -5.f), Vector(0.f, 0.f, 1.f));
    auto xs = sphere.Intersect(ray);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0] == 5.f);
    REQUIRE(xs[1] == 5.f);
}

TEST_CASE("A ray misses a sphere", "[spheres]")
{
    Sphere sphere("sphere");
    Ray ray(Point(0.f, 2.f, -5.f), Vector(0.f, 0.f, 1.f));
    auto xs = sphere.Intersect(ray);

    REQUIRE(xs.size() == 0);
}

TEST_CASE("A ray originates inside a sphere", "[spheres]")
{
    Sphere sphere("sphere");
    Ray ray(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f));
    auto xs = sphere.Intersect(ray);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0] == -1.f);
    REQUIRE(xs[1] == 1.f);
}

TEST_CASE("The sphere is behind the ray", "[spheres]")
{
    Sphere sphere("sphere");
    Ray ray(Point(0.f, 0.f, 5.f), Vector(0.f, 0.f, 1.f));
    auto xs = sphere.Intersect(ray);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0] == -6.f);
    REQUIRE(xs[1] == -4.f);
}

TEST_CASE("A spheres default transformation", "[spheres]")
{
    Sphere sphere("sphere");
    sphere.GetMutableTransform().SetIdentity();
    Matrix expected(4);
    expected.SetIdentity();
    REQUIRE(sphere.GetTransform() == expected);
}

TEST_CASE("Changing a spheres transformation", "[spheres]")
{
    Sphere sphere("sphere");
    Matrix translation = Matrix::CreateTranslation(2.f, 3.f, 4.f);
    sphere.SetTransform(translation);
    Matrix expected = Matrix::CreateTranslation(2.f, 3.f, 4.f);
    REQUIRE(sphere.GetTransform() == expected);
}

TEST_CASE("Intersecting a scaled sphere with a ray", "[spheres]")
{
    Shape testShape("testShape");
    testShape.SetTransform(Matrix::CreateScaling(2.f, 2.f, 2.f));
    Ray ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    auto xs = testShape.Intersect(ray);

    REQUIRE(gSavedRay.GetOrigin() == Point(0.f, 0.f, -2.5f));
    REQUIRE(gSavedRay.GetDirection() == Vector(0.f, 0.f, 0.5f));
}

TEST_CASE("Intersecting a translated sphere with a ray", "[spheres]")
{
    Shape testShape("testShape");
    testShape.SetTransform(Matrix::CreateTranslation(5.f, 0.f, 0.f));
    Ray ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    auto xs = testShape.Intersect(ray);

    REQUIRE(gSavedRay.GetOrigin() == Point(-5.f, 0.f, -5.f));
    REQUIRE(gSavedRay.GetDirection() == Vector(0.f, 0.f, 1.f));
}

TEST_CASE("The normal on a sphere at a point on the x axis", "[spheres]")
{
    Sphere sphere("sphere");
    Tuple normal = sphere.NormalAt(Point(1.f, 0.f, 0.f));
    REQUIRE(normal == Vector(1.f, 0.f, 0.f));
}

TEST_CASE("The normal on a sphere at a point on the y axis", "[spheres]")
{
    Sphere sphere("sphere");
    Tuple normal = sphere.NormalAt(Point(0.f, 1.f, 0.f));
    REQUIRE(normal == Vector(0.f, 1.f, 0.f));
}

TEST_CASE("The normal on a sphere at a point on the z axis", "[spheres]")
{
    Sphere sphere("sphere");
    Tuple normal = sphere.NormalAt(Point(0.f, 0.f, 1.f));
    REQUIRE(normal == Vector(0.f, 0.f, 1.f));
}

TEST_CASE("The normal on a sphere at a nonaxial point", "[spheres]")
{
    Sphere sphere("sphere");
    float rootThreeOverThree = std::sqrt(3.f) / 3.f;
    Tuple normal = sphere.NormalAt(Point(rootThreeOverThree, rootThreeOverThree, rootThreeOverThree));
    REQUIRE(normal == Vector(rootThreeOverThree, rootThreeOverThree, rootThreeOverThree));
}

TEST_CASE("The normal on a sphere at a nonaxial point 2", "[spheres]")
{
    Sphere sphere("sphere");
    float rootThreeOverThree = std::sqrt(3.f) / 3.f;
    Tuple normal = sphere.NormalAt(Point(1.f, 1.f, 1.f));
    REQUIRE(normal == Vector(rootThreeOverThree, rootThreeOverThree, rootThreeOverThree));
}

TEST_CASE("The normal is normalised", "[spheres]")
{
    Sphere sphere("sphere");
    float rootThreeOverThree = std::sqrt(3.f) / 3.f;
    Tuple normal = sphere.NormalAt(Point(1.f, 1.f, 1.f));
    REQUIRE(normal == normal.Normalize());
    REQUIRE(AreEqual(normal.Magnitude(), 1.f));
}

TEST_CASE("Computing the normal on a translated shape", "[spheres]")
{
    Shape shape("shape");
    shape.SetTransform(Matrix::CreateTranslation(0.f, 1.f, 0.f));
    Tuple normal = shape.NormalAt(Point(0.f, 1.70711f, -0.70711f));
    REQUIRE(normal == Vector(0.f, 0.70711f, -0.70711f));
}

TEST_CASE("Computing the normal on a transformed shape", "[spheres]")
{
    Shape shape("shape");
    Matrix scaling = Matrix::CreateScaling(1.f, 0.5f, 1.f);
    Matrix rotation = Matrix::CreateRotationZ(M_PI / 5.f);
    shape.SetTransform(scaling * rotation);
    Tuple normal = shape.NormalAt(Point(0.f, std::sqrt(2.f) / 2.f, -std::sqrt(2.f) / 2.f));
    REQUIRE(normal == Vector(0.f, 0.97014f, -0.24254f));
}

TEST_CASE("A helper for producing a sphere with a glassy material", "[spheres]")
{
    Sphere s = GlassSphere("sphere");
    Matrix required = Matrix(4);
    required.SetIdentity();
    REQUIRE(s.GetTransform() == required);
    REQUIRE(s.GetMaterial().GetTransparency() == 1.f);
    REQUIRE(s.GetMaterial().GetRefractiveIndex() == 1.5f);
}
