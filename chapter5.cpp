#include <iostream>

#include "matrix.hpp"
#include "tuple.hpp"
#include "utils.hpp"
#include "canvas.hpp"
#include "intersection.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "maths.hpp"
#include "renderer.hpp"

static constexpr int kCanvasSize = 200;

int main()
{
    std::cout << "Chapter 5 challenge !!!" << std::endl;

    Canvas canvas{kCanvasSize, kCanvasSize};
    float invCanvas = 1.f / kCanvasSize;
    Color red{1.f, 0.f, 0.f};

    Sphere sphere;

    for (int x = 0; x < kCanvasSize; x++)
    {
        for (int y = 0; y < kCanvasSize; y++)
        {
            Ray ray(Point(0.f, 0.f, -3.f), Vector((x - kCanvasSize / 2.f) * invCanvas, (y - kCanvasSize / 2.f) * invCanvas, 1.f));
            auto xs = Intersect(sphere, ray);
            if (!xs.empty())
            {
                canvas.WritePixel(x, y, red);
            }
        }
    }

    canvas.WriteToPPM("../images/chapter5.ppm");
    return 0;
}
