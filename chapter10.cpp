#include <iostream>

#include "camera.hpp"
#include "matrix.hpp"
#include "plane.hpp"
#include "renderer.hpp"
#include "sphere.hpp"
#include "tuple.hpp"
#include "world.hpp"

static constexpr int kCanvasSize = 512;

int main()
{
    std::cout << "Chapter 10 challenge !!!" << std::endl;

    Camera camera(kCanvasSize, kCanvasSize, M_PI / 3.0f);
    camera.SetTransform(
        Matrix::ViewTransform(Point(0.0f, 1.5f, -5.0f), Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)));

    World world;

    // Light
    Light light(Point(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
    world.AddLight(light);

    // Floor
    Plane floor("floor");
    // floor.SetTransform(Matrix::CreateScaling(10.0f, 0.02f, 10.0f));
    floor.GetMutableMaterial().SetColor(Color(1.0f, 0.9f, 0.9f));
    floor.GetMutableMaterial().SetSpecular(0.0f);
    floor.GetMutableMaterial().SetPattern(StripePattern(kColorRed, kColorBlue));
    world.AddObject(floor);

    // Middle sphere
    Sphere middle("middle");
    middle.SetTransform(Matrix::CreateTranslation(-0.5f, 1.0f, 0.5f));
    middle.GetMutableMaterial().SetColor(Color(0.1f, 1.0f, 0.5f));
    middle.GetMutableMaterial().SetDiffuse(0.7f);
    middle.GetMutableMaterial().SetSpecular(0.3f);
    StripePattern pattern = StripePattern(kColorGreen, kColorYellow);
    pattern.SetTransform(Matrix::CreateScaling(0.25f, 0.25f, 0.25f) * Matrix::CreateRotationY(M_PI / 4.f));
    middle.GetMutableMaterial().SetPattern(pattern);
    world.AddObject(middle);

    // Right sphere
    Sphere right("right");
    right.SetTransform(Matrix::CreateTranslation(2.f, 0.5f, -0.5f) * Matrix::CreateScaling(0.5f, 0.5f, 0.5f));
    right.GetMutableMaterial().SetColor(Color(0.5f, 1.0f, 0.1f));
    right.GetMutableMaterial().SetDiffuse(0.7f);
    right.GetMutableMaterial().SetSpecular(0.3f);
    StripePattern pattern2 = StripePattern(kColorWhite, kColorBlack);
    pattern2.SetTransform(Matrix::CreateScaling(0.05f, 0.05f, 0.05f) * Matrix::CreateRotationZ(M_PI / 7.f));
    right.GetMutableMaterial().SetPattern(pattern2);
    world.AddObject(right);

    // Left sphere
    Sphere left("left");
    left.SetTransform(Matrix::CreateTranslation(-2.f, 0.33f, -0.75f) * Matrix::CreateScaling(0.33f, 0.33f, 0.33f));
    left.GetMutableMaterial().SetColor(Color(1.0f, 0.8f, 0.1f));
    left.GetMutableMaterial().SetDiffuse(0.7f);
    left.GetMutableMaterial().SetSpecular(0.3f);
    StripePattern pattern3 = StripePattern(kColorCyan, kColorMagenta);
    pattern3.SetTransform(Matrix::CreateScaling(0.5f, 0.5f, 0.5f) * Matrix::CreateRotationX(M_PI / 2.f));
    left.GetMutableMaterial().SetPattern(pattern3);
    world.AddObject(left);

    Canvas canvas = Renderer::Render(camera, world);
    canvas.WriteToPPM("../images/chapter10.ppm");
    std::cout << "Done" << std::endl;
    return 0;
}
