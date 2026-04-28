
#include "json/writer.h"
#include <iostream>

#include "camera.hpp"
#include "matrix.hpp"
#include "patterns/checker_pattern.hpp"
#include "renderer.hpp"
#include "shapes/group.hpp"
#include "shapes/plane.hpp"
#include "shapes/sphere.hpp"
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

    // Floor
    {
        Plane floor("floor");
        CheckerPattern check = CheckerPattern(kColorYellow, kColorBlue);
        check.SetTransform(Matrix::CreateTranslation(0.f, 0.5f, 0.f));
        floor.GetMutableMaterial().SetSpecular(0.0f).SetPattern(check);
        world.AddObject(floor);
    }

    // sphere
    {
        Sphere sphere("sphere1");
        sphere.SetTransform(Matrix::CreateTranslation(2.f, 2.0f, 0.f) * Matrix::CreateRotationX(0.5f));
        CheckerPattern check = CheckerPattern(kColorRed, kColorWhite);
        check.SetTransform(Matrix::CreateScaling(0.5f, 0.5f, 0.5f));
        Material mat;
        mat.SetPattern(check);
        sphere.SetMaterial(mat);
        world.AddObject(sphere);
    }

    {
        Group group("group");
        group.SetTransform(Matrix::CreateTranslation(-2.0f, 2.0f, 0.0f) * Matrix::CreateRotationX(0.5f));
        Sphere sphere("sphere2");
        CheckerPattern check = CheckerPattern(kColorGreen, kColorWhite);
        check.SetTransform(Matrix::CreateScaling(0.5f, 0.5f, 0.5f));
        Material mat;
        mat.SetPattern(check);
        sphere.SetMaterial(mat);
        group.AddChild(sphere);
        world.AddObject(group);
    }

    // Serialize and print out world
    Json::Value root;
    world.Serialize(root["world"]);
    Json::StreamWriterBuilder writerBuilder;
    writerBuilder["indentation"] = "  ";
    std::string json = Json::writeString(writerBuilder, root);
    std::cout << json << std::endl;

    Canvas canvas = Renderer::Render(camera, world);
    canvas.WriteToPPM("../images/chapter14.ppm");
    std::cout << "Done" << std::endl;
    return 0;
}
