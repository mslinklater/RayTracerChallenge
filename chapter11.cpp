#include <iostream>

#include "camera.hpp"
#include "matrix.hpp"
#include "patterns/checker_pattern.hpp"
#include "patterns/gradient_pattern.hpp"
#include "patterns/ring_pattern.hpp"
#include "patterns/stripe_pattern.hpp"
#include "plane.hpp"
#include "renderer.hpp"
#include "sphere.hpp"
#include "tuple.hpp"
#include "world.hpp"

static constexpr int kCanvasSize = 1024;

int main()
{
    std::cout << "Chapter 11 challenge !!!" << std::endl;

    Camera camera(kCanvasSize, kCanvasSize, M_PI / 3.0f);
    camera.SetTransform(
        Matrix::ViewTransform(Point(2.0f, 5.0f, -5.0f), Point(0.0f, 1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)));

    World world;

    // Light
    Light light(Point(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
    world.AddLight(light);

    // Floor
    {
        Plane floor("floor");
        floor.GetMutableMaterial().SetColor(Color(1.0f, 0.9f, 0.9f));
        floor.GetMutableMaterial().SetSpecular(0.0f);
        CheckerPattern check = CheckerPattern(kColorYellow, kColorGreen);
        check.SetTransform(Matrix::CreateTranslation(0.f, 0.5f, 0.f));
        floor.GetMutableMaterial().SetPattern(check);
        world.AddObject(floor);
    }

    // Plain sphere
    {
        Sphere sphere("plain");
        sphere.SetTransform(Matrix::CreateTranslation(0.f, 1.0f, 0.f));
        Material mat;
        mat.SetColor(Color(0.7f, 0.7f, 0.7f));
        mat.SetDiffuse(0.7f);
        mat.SetSpecular(0.3f);
        sphere.SetMaterial(mat);
        world.AddObject(sphere);
    }

    // Striped sphere
    {
        Sphere sphere("striped");
        sphere.SetTransform(Matrix::CreateTranslation(2.5f, 1.0f, 0.f));
        Material mat;
        mat.SetColor(Color(0.7f, 0.7f, 0.7f));
        mat.SetDiffuse(0.7f);
        mat.SetSpecular(0.3f);
        StripePattern stripe = StripePattern(kColorRed, kColorBlue);
        stripe.SetTransform(Matrix::CreateScaling(0.25f, 0.25f, 0.25f) * Matrix::CreateRotationY(M_PI / 3.f));
        mat.SetPattern(stripe);
        sphere.SetMaterial(mat);
        world.AddObject(sphere);
    }

    // Checker sphere
    {
        Sphere sphere("checker");
        sphere.SetTransform(Matrix::CreateTranslation(-2.5f, 1.0f, 0.f));
        Material mat;
        mat.SetColor(Color(0.7f, 0.7f, 0.7f));
        mat.SetDiffuse(0.7f);
        mat.SetSpecular(0.3f);
        CheckerPattern pattern = CheckerPattern(kColorCyan, kColorMagenta);
        pattern.SetTransform(Matrix::CreateScaling(0.2f, 0.2f, 0.2f) * Matrix::CreateRotationY(M_PI / 3.f));
        mat.SetPattern(pattern);
        sphere.SetMaterial(mat);
        world.AddObject(sphere);
    }

    // Ring sphere
    {
        Sphere sphere("ring");
        sphere.SetTransform(Matrix::CreateTranslation(0.f, 1.0f, 2.5f));
        Material mat;
        mat.SetColor(Color(0.7f, 0.7f, 0.7f));
        mat.SetDiffuse(0.7f);
        mat.SetSpecular(0.3f);
        RingPattern pattern = RingPattern(kColorBlack, kColorWhite);
        pattern.SetTransform(Matrix::CreateScaling(0.21f, 0.21f, 0.21f) * Matrix::CreateRotationZ(M_PI / 5.f));
        mat.SetPattern(pattern);
        sphere.SetMaterial(mat);
        world.AddObject(sphere);
    }

    // Gradient sphere
    {
        Sphere sphere("gradient");
        sphere.SetTransform(Matrix::CreateTranslation(0.f, 1.0f, -2.5f));
        Material mat;
        mat.SetColor(Color(0.7f, 0.7f, 0.7f));
        mat.SetDiffuse(0.7f);
        mat.SetSpecular(0.3f);
        GradientPattern pattern = GradientPattern(kColorYellow, kColorRed);
        pattern.SetTransform(Matrix::CreateScaling(0.5f, 0.5f, 0.5f) * Matrix::CreateRotationX(M_PI / 3.f));
        mat.SetPattern(pattern);
        sphere.SetMaterial(mat);
        world.AddObject(sphere);
    }

    // Mirror sphere
    {
        Sphere sphere("mirror1");
        sphere.SetTransform(Matrix::CreateTranslation(2.5f, 1.0f, -2.5f));
        Material mat;
        mat.SetColor(Color(0.7f, 0.7f, 0.7f));
        mat.SetDiffuse(0.7f);
        mat.SetSpecular(0.3f);
        mat.SetReflective(1.0f);
        sphere.SetMaterial(mat);
        world.AddObject(sphere);
    }

    // Mirror sphere
    {
        Sphere sphere("mirror2");
        sphere.SetTransform(Matrix::CreateTranslation(2.5f, 1.0f, 2.5f));
        Material mat;
        mat.SetColor(Color(0.7f, 0.7f, 0.7f));
        mat.SetDiffuse(0.7f);
        mat.SetSpecular(0.3f);
        mat.SetReflective(1.0f);
        sphere.SetMaterial(mat);
        world.AddObject(sphere);
    }

    // Mirror sphere
    {
        Sphere sphere("Mirror3");
        sphere.SetTransform(Matrix::CreateTranslation(-2.5f, 1.0f, 2.5f));
        Material mat;
        mat.SetColor(Color(0.7f, 0.7f, 0.7f));
        mat.SetDiffuse(0.7f);
        mat.SetSpecular(0.3f);
        mat.SetReflective(1.0f);
        sphere.SetMaterial(mat);
        world.AddObject(sphere);
    }

    // Mirror sphere
    {
        Sphere sphere("mirror4");
        sphere.SetTransform(Matrix::CreateTranslation(-2.5f, 1.0f, -2.5f));
        Material mat;
        mat.SetColor(Color(0.7f, 0.7f, 0.7f));
        mat.SetDiffuse(0.7f);
        mat.SetSpecular(0.3f);
        mat.SetReflective(1.0f);
        sphere.SetMaterial(mat);
        world.AddObject(sphere);
    }

    Canvas canvas = Renderer::Render(camera, world);
    canvas.WriteToPPM("../images/chapter11.ppm");
    std::cout << "Done" << std::endl;
    return 0;
}
