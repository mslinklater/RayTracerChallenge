#include <iostream>

#include "matrix.hpp"
#include "tuple.hpp"
#include "camera.hpp"
#include "renderer.hpp"
#include "world.hpp"

static constexpr int kCanvasSize = 100;

int main()
{
    std::cout << "Chapter 7 challenge !!!" << std::endl;

    Camera camera(kCanvasSize, kCanvasSize, M_PI / 2.0f);
    camera.SetTransform(Matrix::ViewTransform(Point(0.0f, 0.0f, -5.0f), Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)));

    World world;

    // Light
    Light light(Point(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
    world.AddLight(light);

    // Floor
    Sphere floor;
    floor.SetTransform(Matrix::CreateScaling(10.0f, 0.01f, 10.0f));
    floor.GetMutableMaterial().SetColor(Color(1.0f, 0.9f, 0.9f));
    floor.GetMutableMaterial().SetSpecular(0.0f);
    world.AddObject(floor);

    // Left wall
    Sphere leftWall;
    leftWall.SetTransform(Matrix::CreateTranslation(0.0f, 0.0f, 5.0f) * Matrix::CreateRotationY(-M_PI / 4.0f) * Matrix::CreateRotationX(M_PI / 2.0f) * Matrix::CreateScaling(10.0f, 0.01f, 10.0f));
    leftWall.SetMaterial(floor.GetMaterial());
    world.AddObject(leftWall);

    Canvas canvas = Render(camera, world);
    canvas.WriteToPPM("../images/chapter7.ppm");
    std::cout << "Done" << std::endl;
    return 0;
}
