#include <iostream>

#include "canvas.hpp"
#include "maths.hpp"
#include "matrix.hpp"
#include "tuple.hpp"

static constexpr int kCanvasSize = 200;

int main()
{
    std::cout << "Chapter 4 challenge !!!" << std::endl;

    Canvas canvas{kCanvasSize, kCanvasSize};
    Color white{1.f, 1.f, 1.f};

    for (int i = 0; i < 12; i++)
    {
        Tuple point = Point(0.f, 0.f, 0.f);
        Matrix radiusT = Matrix::CreateTranslation(kCanvasSize * 0.4f, 0.f, 0.f);
        Matrix rotationT = Matrix::CreateRotationZ(i * 30.f * kDegreesToRadians);
        Matrix centerT = Matrix::CreateTranslation(kCanvasSize * 0.5f, kCanvasSize * 0.5f, 0.f);
        point = centerT * rotationT * radiusT * point;

        canvas.WritePixel(point.x, point.y, white);
    }

    canvas.WriteToPPM("../images/chapter4.ppm");
    std::cout << "Done!" << std::endl;
    return 0;
}
