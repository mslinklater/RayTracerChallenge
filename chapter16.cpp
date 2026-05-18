
#include "json/writer.h"
#include <iostream>

#include "camera.hpp"
#include "matrix.hpp"
#include "patterns/checker_pattern.hpp"
#include "renderer.hpp"
#include "shapes/csg.hpp"
#include "shapes/cube.hpp"
#include "shapes/plane.hpp"
#include "shapes/sphere.hpp"
#include "tuple.hpp"
#include "world.hpp"

static constexpr int kCanvasSize = 2048;

int main()
{
    std::cout << "Chapter 16 challenge !!!" << std::endl;

    Camera camera(kCanvasSize * (16.f / 9.f), kCanvasSize, M_PI / 4.0f);
    camera.SetTransform(
        Matrix::ViewTransform(Point(3.0f, 4.0f, -6.0f), Point(0.0f, 1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)));

    World world;

    // Light
    Light lightr(Point(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
    world.AddLight(lightr);
    Light lightl(Point(10.0f, 5.0f, -10.0f), Color(0.5f, 0.5f, 0.5f));
    world.AddLight(lightl);

    // Floor
    {
        Plane floor("floor");
        CheckerPattern check = CheckerPattern(kColorYellow, kColorBlue);
        check.SetTransform(Matrix::CreateTranslation(0.f, 0.5f, 0.f));
        floor.GetMutableMaterial().SetSpecular(0.0f).SetPattern(check);
        world.AddObject(floor);
    }

    // CSG

    Cube cube1("cube1");
    cube1.GetMutableMaterial().SetReflective(0.3f);

    Sphere sphere1("sphere1");
    sphere1.SetTransform(Matrix::CreateScaling(1.1f, 1.1f, 1.1f));
    sphere1.GetMutableMaterial().SetReflective(0.3f);

    CSG csg("csg", CSG::OpDifference, &cube1, &sphere1);
    csg.SetTransform(Matrix::CreateTranslation(0.f, 1.2f, 0.f) * Matrix::CreateRotationY(M_PI / 7.f));
    world.AddObject(csg);

    // Serialize and print out world
    Json::Value root;
    world.Serialize(root["world"]);
    Json::StreamWriterBuilder writerBuilder;
    writerBuilder["indentation"] = "  ";
    std::string json = Json::writeString(writerBuilder, root);
    std::cout << json << std::endl;

    Canvas canvas = Renderer::Render(camera, world);
    canvas.WriteToPPM("../images/chapter16.ppm");
    std::cout << "Done" << std::endl;
    return 0;
}
