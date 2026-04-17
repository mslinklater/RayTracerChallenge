#include "camera.hpp"
#include "canvas.hpp"
#include "maths.hpp"
#include "ray.hpp"
#include "renderer.hpp"
#include "world.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Constructing a camera", "[camera]")
{
    const int hsize = 160;
    const int vsize = 120;
    const float fieldOfView = kPi / 2.f;
    const Camera c(hsize, vsize, fieldOfView);
    Matrix identity(4);
    identity.SetIdentity();
    REQUIRE(c.GetHSize() == hsize);
    REQUIRE(c.GetVSize() == vsize);
    REQUIRE(c.GetFieldOfView() == fieldOfView);
    REQUIRE(c.GetTransform() == identity);
}

TEST_CASE("A pixel size for a horizontal canvas", "[camera]")
{
    const Camera c(200, 125, kPi / 2.f);
    REQUIRE(c.GetPixelSize() == 0.01f);
}

TEST_CASE("A pixel size for a vertical canvas", "[camera]")
{
    const Camera c(125, 200, kPi / 2.f);
    REQUIRE(c.GetPixelSize() == 0.01f);
}

TEST_CASE("A pixel size for a square canvas", "[camera]")
{
    const Camera c(200, 200, kPi / 2.f);
    REQUIRE(c.GetPixelSize() == 0.01f);
}

TEST_CASE("Constructing a ray through the center of the canvas", "[camera]")
{
    const Camera c(201, 101, kPi / 2.f);
    const Ray r = c.RayForPixel(100, 50);
    REQUIRE(r.GetOrigin() == Point(0.f, 0.f, 0.f));
    REQUIRE(r.GetDirection() == Vector(0.f, 0.f, -1.f));
}

TEST_CASE("Constructing a ray through a corner of the canvas", "[camera]")
{
    const Camera c(201, 101, kPi / 2.f);
    const Ray r = c.RayForPixel(0, 0);
    REQUIRE(r.GetOrigin() == Point(0.f, 0.f, 0.f));
    REQUIRE(r.GetDirection() == Vector(0.66519f, 0.33259f, -0.66851f));
}

TEST_CASE("Constructing a ray when the camera is transformed", "[camera]")
{
    Camera c(201, 101, kPi / 2.f);
    Matrix transform = Matrix::CreateRotationY(kPi / 4.f) * Matrix::CreateTranslation(0.f, -2.f, 5.f);
    c.SetTransform(transform);
    const Ray r = c.RayForPixel(100, 50);
    REQUIRE(r.GetOrigin() == Point(0.f, 2.f, -5.f));
    REQUIRE(r.GetDirection() == Vector(std::sqrt(2.f) / 2.f, 0.f, -std::sqrt(2.f) / 2.f));
}

TEST_CASE("Rendering a world with a camera", "[camera]")
{
    World w = Renderer::DefaultWorld();
    Camera c(11, 11, kPi / 2.f);
    const Tuple from = Point(0.f, 0.f, -5.f);
    const Tuple to = Point(0.f, 0.f, 0.f);
    const Tuple up = Vector(0.f, 1.f, 0.f);
    c.SetTransform(Matrix::ViewTransform(from, to, up));
    Canvas canvas = Renderer::Render(c, w);
    REQUIRE(canvas.GetPixel(5, 5) == Color(0.38066f, 0.47583f, 0.2855f));
}
