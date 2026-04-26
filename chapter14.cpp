
#include <iostream>

#include "camera.hpp"
#include "matrix.hpp"
#include "patterns/checker_pattern.hpp"
#include "renderer.hpp"
#include "shapes/cone.hpp"
#include "shapes/cylinder.hpp"
#include "shapes/plane.hpp"
#include "tuple.hpp"
#include "world.hpp"

static constexpr int kCanvasSize = 1024;

int main()
{
    std::cout << "Chapter 14 challenge !!!" << std::endl;

    Camera camera(kCanvasSize * (16.f / 9.f), kCanvasSize, M_PI / 3.0f);
    camera.SetTransform(
        Matrix::ViewTransform(Point(2.0f, 5.0f, -5.0f), Point(0.0f, 1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)));

    World world;

    // Light
    Light lightr(Point(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
    world.AddLight(lightr);
    // Light 2
    Light lightg(Point(10.0f, 10.0f, -10.0f), Color(0.8f, 0.8f, 0.8f));
    world.AddLight(lightg);
    // Light 2
    Light lightb(Point(10.0f, 20.0f, 10.0f), Color(0.5f, 0.5f, 0.5f));
    world.AddLight(lightb);

    // Floor
    {
        Plane floor("floor");
        CheckerPattern check = CheckerPattern(kColorYellow, kColorBlue);
        check.SetTransform(Matrix::CreateTranslation(0.f, 0.5f, 0.f));
        floor.GetMutableMaterial().SetSpecular(0.0f).SetPattern(check);
        world.AddObject(floor);
    }
    // cylinder
    {
        Cylinder cylinder("cylinder1");
        cylinder.SetMinimum(-1.f);
        cylinder.SetMaximum(1.f);
        cylinder.SetTransform(Matrix::CreateTranslation(0.f, 1.0f, 0.f) * Matrix::CreateRotationX(0.f));
        cylinder.SetClosed(true);
        Material mat;
        mat.SetColor(Color(1.0f, 1.0f, 1.0f)).SetDiffuse(0.2f).SetSpecular(0.3f);
        mat.SetReflective(0.5f);
        cylinder.SetMaterial(mat);
        world.AddObject(cylinder);
    }
    // cone
    {
        Cone cone("cone");
        cone.SetTransform(Matrix::CreateTranslation(2.f, 1.0f, 0.f) * Matrix::CreateScaling(0.5f, 0.5f, 1.0f) *
                          Matrix::CreateRotationX(1.f));
        cone.SetMinimum(-3.f);
        cone.SetMaximum(3.f);
        cone.SetClosed(true);
        Material mat;
        mat.SetColor(Color(1.0f, 0.0f, 0.0f)).SetDiffuse(0.2f).SetSpecular(0.3f);
        mat.SetReflective(0.5f);
        cone.SetMaterial(mat);
        world.AddObject(cone);
    }
    // cylinder
    {
        Cylinder cylinder("cylinder3");
        cylinder.SetTransform(Matrix::CreateTranslation(-2.f, 1.0f, 0.f) * Matrix::CreateRotationX(2.f));
        cylinder.SetMinimum(-1.f);
        cylinder.SetMaximum(1.f);
        cylinder.SetClosed(true);
        Material mat;
        mat.SetColor(Color(1.0f, 1.0f, 1.0f)).SetDiffuse(0.2f).SetSpecular(0.3f);
        mat.SetReflective(0.5f);
        cylinder.SetMaterial(mat);
        world.AddObject(cylinder);
    }

    Canvas canvas = Renderer::Render(camera, world);
    canvas.WriteToPPM("../images/chapter14.ppm");
    std::cout << "Done" << std::endl;
    return 0;
}
