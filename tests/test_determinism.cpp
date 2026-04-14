#include "camera.hpp"
#include "canvas.hpp"
#include "maths.hpp"
#include "matrix.hpp"
#include "renderer.hpp"
#include "shapes/plane.hpp"
#include "shapes/sphere.hpp"
#include "world.hpp"
#include <catch2/catch_test_macros.hpp>
#include <array>
#include <optional>
#include <thread>

namespace
{
Camera CreateDeterminismCamera()
{
    const int kCanvasSize = 32;
    Camera camera(kCanvasSize, kCanvasSize, kPi / 3.0f);
    camera.SetTransform(
        Matrix::ViewTransform(Point(0.0f, 1.5f, -5.0f), Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)));
    return camera;
}

World CreateDeterminismWorld()
{
    World world;

    Light light(Point(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
    world.AddLight(light);

    Plane floor("floor");
    floor.GetMutableMaterial().SetColor(Color(1.0f, 0.9f, 0.9f));
    floor.GetMutableMaterial().SetSpecular(0.0f);
    world.AddObject(floor);

    Sphere leftWall("leftWall");
    leftWall.SetTransform(Matrix::CreateTranslation(0.0f, 0.0f, 5.0f) * Matrix::CreateRotationY(-kPi / 4.0f) *
                          Matrix::CreateRotationX(kPi / 2.0f) * Matrix::CreateScaling(10.0f, 0.01f, 10.0f));
    leftWall.SetMaterial(floor.GetMaterial());

    Sphere rightWall("rightWall");
    rightWall.SetTransform(Matrix::CreateTranslation(0.0f, 0.0f, 5.0f) * Matrix::CreateRotationY(kPi / 4.0f) *
                           Matrix::CreateRotationX(kPi / 2.0f) * Matrix::CreateScaling(10.0f, 0.02f, 10.0f));
    rightWall.SetMaterial(floor.GetMaterial());

    Sphere middle("middle");
    middle.SetTransform(Matrix::CreateTranslation(-0.5f, 1.0f, 0.5f));
    middle.GetMutableMaterial().SetColor(Color(0.1f, 1.0f, 0.5f));
    middle.GetMutableMaterial().SetDiffuse(0.7f);
    middle.GetMutableMaterial().SetSpecular(0.3f);
    world.AddObject(middle);

    Sphere right("right");
    right.SetTransform(Matrix::CreateTranslation(1.5f, 0.5f, -0.5f) * Matrix::CreateScaling(0.5f, 0.5f, 0.5f));
    right.GetMutableMaterial().SetColor(Color(0.5f, 1.0f, 0.1f));
    right.GetMutableMaterial().SetDiffuse(0.7f);
    right.GetMutableMaterial().SetSpecular(0.3f);
    world.AddObject(right);

    Sphere left("left");
    left.SetTransform(Matrix::CreateTranslation(-1.5f, 0.33f, -0.75f) * Matrix::CreateScaling(0.33f, 0.33f, 0.33f));
    left.GetMutableMaterial().SetColor(Color(1.0f, 0.8f, 0.1f));
    left.GetMutableMaterial().SetDiffuse(0.7f);
    left.GetMutableMaterial().SetSpecular(0.3f);
    world.AddObject(left);

    return world;
}
} // namespace

TEST_CASE("Determism 1", "[determinism]")
{
    const Camera camera = CreateDeterminismCamera();
    const World world = CreateDeterminismWorld();

    Canvas canvas1 = Renderer::Render(camera, world);
    Canvas canvas2 = Renderer::Render(camera, world);
    REQUIRE(canvas1 == canvas2);
}

TEST_CASE("Concurrent rendering produces the same image", "[determinism]")
{
    const Camera camera = CreateDeterminismCamera();
    const World world = CreateDeterminismWorld();
    const Canvas expected = Renderer::Render(camera, world);

    std::array<std::optional<Canvas>, 4> results;
    std::array<std::thread, 4> threads;

    for (size_t index = 0; index < threads.size(); ++index)
    {
        threads[index] = std::thread([&camera, &results, &world, index]() { results[index] = Renderer::Render(camera, world); });
    }

    for (std::thread &thread : threads)
    {
        thread.join();
    }

    for (const std::optional<Canvas> &result : results)
    {
        REQUIRE(result.has_value());
        REQUIRE(result.value() == expected);
    }
}
