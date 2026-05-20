#include "boundingbox.hpp"

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
