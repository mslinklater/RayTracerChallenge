#include "camera.hpp"
#include "maths.hpp"
#include "ray.hpp"
#include <cassert>
#include <cmath>

Camera::Camera(int hsize, int vsize, float fieldOfView)
    : hsize(hsize), vsize(vsize), fieldOfView(fieldOfView), transform(Matrix(4)), inverseTransform(Matrix(4))
{
    assert(hsize > 0);
    assert(vsize > 0);
    assert(fieldOfView > 0.f && fieldOfView < kPi);

    transform.SetIdentity();
    inverseTransform.SetIdentity();

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

const Matrix &Camera::GetTransform() const
{
    return transform;
}

void Camera::SetTransform(const Matrix &transform)
{
    assert(transform.GetSize() == 4);
    assert(transform.IsValid());

    this->transform = transform;
    inverseTransform = transform.GetInverse();
}

float Camera::GetPixelSize() const
{
    return pixelSize;
}

Ray Camera::RayForPixel(int px, int py) const
{
    assert(px >= 0 && px < hsize);
    assert(py >= 0 && py < vsize);

    float xoffset = (static_cast<float>(px) + 0.5f) * pixelSize;
    float yoffset = (static_cast<float>(py) + 0.5f) * pixelSize;

    float worldX = halfWidth - xoffset;
    float worldY = halfHeight - yoffset;

    Tuple pixel = inverseTransform * Point(worldX, worldY, -1.f);
    Tuple origin = inverseTransform * Point(0.f, 0.f, 0.f);
    Tuple direction = (pixel - origin).Normalize();

    return Ray(Point(origin), Vector(direction));
}
