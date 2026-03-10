#include <iostream>

#include "matrix.hpp"
#include "tuple.hpp"
#include "utils.hpp"
#include "canvas.hpp"
#include "intersection.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "maths.hpp"
#include "light.hpp"

static constexpr int kCanvasSize = 500;

int main()
{
    std::cout << "Chapter 6 challenge !!!" << std::endl;

    Canvas canvas{kCanvasSize, kCanvasSize};
    float invCanvas = 1.f / kCanvasSize;

    Sphere sphere;
    Material m;
    m.SetColor(Color(1.f, 0.2f, 1.f));
    sphere.SetMaterial(m);
    Light light(Point(-10.f, 10.f, -10.f), Color(1.f, 1.f, 1.f));

    for (int x = 0; x < kCanvasSize; x++)
    {
        for (int y = 0; y < kCanvasSize; y++)
        {
            Ray ray(Point(0.f, 0.f, -3.f), Vector((x - kCanvasSize / 2.f) * invCanvas, (y - kCanvasSize / 2.f) * invCanvas, 1.f));
            auto xs = Intersect(sphere, ray);
            if (!xs.empty())
            {
                Tuple position = Position(ray, xs[0]);
                Tuple normalVector = sphere.NormalAt(position);
                Tuple eyeVector = -ray.direction;
                Color color = Lighting(sphere.GetMaterial(), light, position, eyeVector, normalVector);
                canvas.WritePixel(x, y, color);
            }
        }
    }

    canvas.WriteToPPM("../images/chapter6.ppm");
    return 0;
}
