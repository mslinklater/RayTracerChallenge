#pragma once
#include "matrix.hpp"
#include "ray.hpp"

class World;

class Camera
{
public:
    Camera(int hsize, int vsize, float fieldOfView);
    int GetHSize() const;
    int GetVSize() const;
    float GetFieldOfView() const;
    Matrix GetTransform() const;
    void SetTransform(const Matrix &transform) { this->transform = transform; }
    float GetPixelSize() const;

    Ray RayForPixel(int px, int py) const;

private:
    int hsize;
    int vsize;
    float fieldOfView;
    Matrix transform;
    float pixelSize;
    float halfWidth;
    float halfHeight;
};
