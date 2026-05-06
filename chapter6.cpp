#include <iostream>

#include "canvas.hpp"
#include "enums.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "renderer.hpp"
#include "shapes/sphere.hpp"
#include "tuple.hpp"

static constexpr int kCanvasSize = 500;

int main()
{
    std::cout << "Chapter 6 challenge !!!" << std::endl;

    Canvas canvas{kCanvasSize, kCanvasSize};
    float invCanvas = 1.f / kCanvasSize;

    Sphere sphere("sphere");
    Material m;
    m.SetColor(Color(1.f, 0.2f, 1.f));
    sphere.SetMaterial(m);
    Light light(Point(-10.f, 10.f, -10.f), Color(1.f, 1.f, 1.f));

    for (int x = 0; x < kCanvasSize; x++)
    {
        for (int y = 0; y < kCanvasSize; y++)
        {
            Ray ray(Point(0.f, 0.f, -3.f),
                    Vector((x - kCanvasSize / 2.f) * invCanvas, (y - kCanvasSize / 2.f) * invCanvas, 1.f));
            auto xs = sphere.Intersect(ray);
            if (!xs.empty())
            {
                Tuple position = ray.PositionAt(xs[0].GetT());
                Tuple normalVector = sphere.NormalAt(position, xs[0]);
                Tuple eyeVector = -ray.GetDirection();
                Color color = Renderer::Lighting(sphere.GetMaterial(), sphere, light, position, eyeVector, normalVector,
                                                 EInShadow::No);
                canvas.WritePixel(x, y, color);
            }
        }
    }

    canvas.WriteToPPM("../images/chapter6.ppm");
    return 0;
}
