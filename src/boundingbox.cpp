#include "boundingbox.hpp"
#include "matrix.hpp"

BoundingBox::BoundingBox()
    : min(Point(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
                std::numeric_limits<float>::max())),
      max(Point(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(),
                -std::numeric_limits<float>::max()))
{
}

BoundingBox::BoundingBox(const Tuple& a, const Tuple& b)
{
    min.x = std::min(a.x, b.x);
    min.y = std::min(a.y, b.y);
    min.z = std::min(a.z, b.z);
    max.x = std::max(a.x, b.x);
    max.y = std::max(a.y, b.y);
    max.z = std::max(a.z, b.z);
    min.w = 1.f;
    max.w = 1.f;
}

void BoundingBox::AddPoint(const Tuple& point)
{
    min.x = std::min(min.x, point.x);
    min.y = std::min(min.y, point.y);
    min.z = std::min(min.z, point.z);

    max.x = std::max(max.x, point.x);
    max.y = std::max(max.y, point.y);
    max.z = std::max(max.z, point.z);
}

BoundingBox operator+(const BoundingBox& b1, const BoundingBox& b2)
{
    return BoundingBox(Point(std::min(b1.GetMin().x, b2.GetMin().x), std::min(b1.GetMin().y, b2.GetMin().y),
                             std::min(b1.GetMin().z, b2.GetMin().z)),
                       Point(std::max(b1.GetMax().x, b2.GetMax().x), std::max(b1.GetMax().y, b2.GetMax().y),
                             std::max(b1.GetMax().z, b2.GetMax().z)));
}

bool BoundingBox::Contains(const Tuple& point) const
{
    return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y && point.z >= min.z &&
           point.z <= max.z;
}

bool BoundingBox::Contains(const BoundingBox& box) const
{
    return Contains(box.GetMin()) && Contains(box.GetMax());
}

BoundingBox BoundingBox::Transform(const Matrix& matrix) const
{
    // Transform all 8 corners of the box and return a new box that contains them
    BoundingBox result;
    for (int x = 0; x <= 1; ++x)
    {
        for (int y = 0; y <= 1; ++y)
        {
            for (int z = 0; z <= 1; ++z)
            {
                Tuple corner =
                    Point(min.x + x * (max.x - min.x), min.y + y * (max.y - min.y), min.z + z * (max.z - min.z));
                result.AddPoint(matrix * corner);
            }
        }
    }
    return result;
}

bool BoundingBox::Intersect(const Ray& ray) const
{
    float tmin = (min.x - ray.GetOrigin().x) / ray.GetDirection().x;
    float tmax = (max.x - ray.GetOrigin().x) / ray.GetDirection().x;

    if (tmin > tmax)
    {
        std::swap(tmin, tmax);
    }

    float tymin = (min.y - ray.GetOrigin().y) / ray.GetDirection().y;
    float tymax = (max.y - ray.GetOrigin().y) / ray.GetDirection().y;

    if (tymin > tymax)
    {
        std::swap(tymin, tymax);
    }

    if ((tmin > tymax) || (tymin > tmax))
    {
        return false;
    }

    if (tymin > tmin)
    {
        tmin = tymin;
    }

    if (tymax < tmax)
    {
        tmax = tymax;
    }

    float tzmin = (min.z - ray.GetOrigin().z) / ray.GetDirection().z;
    float tzmax = (max.z - ray.GetOrigin().z) / ray.GetDirection().z;

    if (tzmin > tzmax)
    {
        std::swap(tzmin, tzmax);
    }

    if ((tmin > tzmax) || (tzmin > tmax))
    {
        return false;
    }

    return true;
}
