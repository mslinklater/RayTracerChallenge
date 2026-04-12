#include <iostream>

#include "canvas.hpp"
#include "ray.hpp"
#include "renderer.hpp"
#include "shapes/sphere.hpp"
#include "tuple.hpp"

static constexpr int kCanvasSize = 200;

int main()
{
    std::cout << "Chapter 5 challenge !!!" << std::endl;

    Canvas canvas{kCanvasSize, kCanvasSize};
    float invCanvas = 1.f / kCanvasSize;
    Color red{1.f, 0.f, 0.f};

    Sphere sphere("sphere");

    for (int x = 0; x < kCanvasSize; x++)
    {
        for (int y = 0; y < kCanvasSize; y++)
        {
            Ray ray(Point(0.f, 0.f, -3.f),
                    Vector((x - kCanvasSize / 2.f) * invCanvas, (y - kCanvasSize / 2.f) * invCanvas, 1.f));
            auto xs = sphere.Intersect(ray);
            if (!xs.empty())
            {
                canvas.WritePixel(x, y, red);
            }
        }
    }

    canvas.WriteToPPM("../images/chapter5.ppm");
    return 0;
}
