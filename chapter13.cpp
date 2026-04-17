
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
    std::cout << "Chapter 13 challenge !!!" << std::endl;

    Camera camera(kCanvasSize, kCanvasSize, M_PI / 3.0f);
    camera.SetTransform(
        Matrix::ViewTransform(Point(2.0f, 5.0f, -5.0f), Point(0.0f, 1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)));

    World world;

    // Light
    Light lightr(Point(-10.0f, 10.0f, -10.0f), Color(1.0f, 0.0f, 0.0f));
    world.AddLight(lightr);
    // Light 2
    Light lightg(Point(10.0f, 10.0f, -10.0f), Color(0.0f, 1.0f, 0.0f));
    world.AddLight(lightg);
    // Light 2
    Light lightb(Point(10.0f, 20.0f, 10.0f), Color(0.0f, 0.0f, 1.0f));
    world.AddLight(lightb);

    // Floor
    {
        Plane floor("floor");
        CheckerPattern check = CheckerPattern(kColorYellow, kColorGreen);
        check.SetTransform(Matrix::CreateTranslation(0.f, 0.5f, 0.f));
        floor.GetMutableMaterial().SetSpecular(0.0f).SetPattern(check);
        world.AddObject(floor);
    }

    Canvas canvas = Renderer::Render(camera, world);
    canvas.WriteToPPM("../images/chapter13.ppm");
    std::cout << "Done" << std::endl;
    return 0;
}
