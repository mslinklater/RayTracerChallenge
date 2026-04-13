
#include <iostream>

#include "camera.hpp"
#include "matrix.hpp"
#include "patterns/checker_pattern.hpp"
#include "renderer.hpp"
#include "shapes/cube.hpp"
#include "shapes/plane.hpp"
#include "tuple.hpp"
#include "world.hpp"

static constexpr int kCanvasSize = 256;

int main()
{
    std::cout << "Chapter 12 challenge !!!" << std::endl;

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
        CheckerPattern check = CheckerPattern(kColorYellow, kColorGreen);
        check.SetTransform(Matrix::CreateTranslation(0.f, 0.5f, 0.f));
        floor.GetMutableMaterial().SetSpecular(0.0f).SetPattern(check);
        world.AddObject(floor);
    }

    // cube
    {
        Cube cube("cube_center");
        cube.SetTransform(Matrix::CreateTranslation(0.0f, 1.0f, 0.0f) * Matrix::CreateRotationY(M_PI / 3.0f));
        world.AddObject(cube);
    }
    {
        Cube cube("cube_top");
        cube.SetTransform(Matrix::CreateTranslation(0.0f, 1.0f, 3.0f) * Matrix::CreateRotationX(M_PI / 3.0f));
        Material mat;
        mat.SetColor(kColorRed).SetSpecular(0.9).SetTransparency(0.9f).SetRefractiveIndex(1.5f);
        cube.SetMaterial(mat);
        world.AddObject(cube);
    }
    {
        Cube cube("cube_bottom");
        cube.SetTransform(Matrix::CreateTranslation(0.0f, 1.0f, -3.0f) * Matrix::CreateRotationZ(M_PI / 3.0f));
        CheckerPattern check = CheckerPattern(kColorBlue, kColorWhite);
        check.SetTransform(Matrix::CreateScaling(0.2f, 0.2f, 0.2f) * Matrix::CreateTranslation(0.5f, 0.5f, 0.5f));
        Material mat;
        mat.SetPattern(check);
        cube.SetMaterial(mat);
        world.AddObject(cube);
    }

    Canvas canvas = Renderer::Render(camera, world);
    canvas.WriteToPPM("../images/chapter12.ppm");
    std::cout << "Done" << std::endl;
    return 0;
}
