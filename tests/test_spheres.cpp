#include <catch2/catch_test_macros.hpp>
#include "sphere.hpp"
#include "ray.hpp"

TEST_CASE("A ray intersects a sphere at two points", "[Spheres]")
{
    Sphere sphere;
    Ray ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    auto xs = Intersect(sphere, ray);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0] == 4.f);
    REQUIRE(xs[1] == 6.f);
}

TEST_CASE("A ray intersects a sphere at tangent - two identical points", "[Spheres]")
{
    Sphere sphere;
    Ray ray(Point(0.f, 1.f, -5.f), Vector(0.f, 0.f, 1.f));
    auto xs = Intersect(sphere, ray);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0] == 5.f);
    REQUIRE(xs[1] == 5.f);
}

TEST_CASE("A ray misses a sphere", "[Spheres]")
{
    Sphere sphere;
    Ray ray(Point(0.f, 2.f, -5.f), Vector(0.f, 0.f, 1.f));
    auto xs = Intersect(sphere, ray);

    REQUIRE(xs.size() == 0);
}

TEST_CASE("A ray originates inside a sphere", "[Spheres]")
{
    Sphere sphere;
    Ray ray(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f));
    auto xs = Intersect(sphere, ray);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0] == -1.f);
    REQUIRE(xs[1] == 1.f);
}

TEST_CASE("The sphere is behind the ray", "[Spheres]")
{
    Sphere sphere;
    Ray ray(Point(0.f, 0.f, 5.f), Vector(0.f, 0.f, 1.f));
    auto xs = Intersect(sphere, ray);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0] == -6.f);
    REQUIRE(xs[1] == -4.f);
}

TEST_CASE("A spheres default transformation", "[Spheres]")
{
    Sphere sphere;
    sphere.transform.SetIdentity();
    Matrix expected(4);
    expected.SetIdentity();
    REQUIRE(sphere.transform == expected);
}

TEST_CASE("Changing a spheres transformation", "[Spheres]")
{
    Sphere sphere;
    Matrix translation = Matrix::CreateTranslation(2.f, 3.f, 4.f);
    sphere.transform = translation;
    Matrix expected = Matrix::CreateTranslation(2.f, 3.f, 4.f);
    REQUIRE(sphere.transform == expected);
}

TEST_CASE("Intersecting a scaled sphere with a ray", "[Spheres]")
{
    Sphere sphere;
    sphere.transform = Matrix::CreateScaling(2.f, 2.f, 2.f);
    Ray ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    auto xs = Intersect(sphere, ray);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0] == 3.f);
    REQUIRE(xs[1] == 7.f);
}

TEST_CASE("Intersecting a translated sphere with a ray", "[Spheres]")
{
    Sphere sphere;
    sphere.transform = Matrix::CreateTranslation(5.f, 0.f, 0.f);
    Ray ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    auto xs = Intersect(sphere, ray);

    REQUIRE(xs.size() == 0);
}