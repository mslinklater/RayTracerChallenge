#include "camera.hpp"
#include "ray.hpp"
#include <cmath>

Camera::Camera(int hsize, int vsize, float fieldOfView)
    : hsize(hsize), vsize(vsize), fieldOfView(fieldOfView), transform(Matrix(4))
{
    transform.SetIdentity();

    float halfView = std::tan(fieldOfView / 2.f);
    float aspect = static_cast<float>(hsize) / static_cast<float>(vsize);

    if (aspect >= 1.f)
    {
        halfWidth = halfView;
        halfHeight = halfView / aspect;
    }
    else
    {
        halfWidth = halfView * aspect;
        halfHeight = halfView;
    }
    pixelSize = (halfWidth * 2.f) / static_cast<float>(hsize);
}

int Camera::GetHSize() const
{
    return hsize;
}

int Camera::GetVSize() const
{
    return vsize;
}

float Camera::GetFieldOfView() const
{
    return fieldOfView;
}

Matrix Camera::GetTransform() const
{
    return transform;
}

float Camera::GetPixelSize() const
{
    return pixelSize;
}

Ray Camera::RayForPixel(int px, int py) const
{
    float xoffset = (static_cast<float>(px) + 0.5f) * pixelSize;
    float yoffset = (static_cast<float>(py) + 0.5f) * pixelSize;

    float worldX = halfWidth - xoffset;
    float worldY = halfHeight - yoffset;

    Matrix invTransform = transform.GetInverse();
    Tuple pixel = invTransform * Point(worldX, worldY, -1.f);
    Tuple origin = invTransform * Point(0.f, 0.f, 0.f);
    Tuple direction = (pixel - origin).Normalize();

    return Ray(origin, direction);
}
