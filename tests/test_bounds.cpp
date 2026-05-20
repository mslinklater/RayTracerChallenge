#include "boundingbox.hpp"
#include "shapes/cone.hpp"
#include "shapes/cube.hpp"
#include "shapes/cylinder.hpp"
#include "shapes/triangle.hpp"
#include "tuple.hpp"
#include <catch2/catch_test_macros.hpp>

class TestShape : public Shape
{
  public:
    TestShape(const std::string& name) : Shape(name)
    {
    }

    std::unique_ptr<Shape> Clone() const override
    {
        return std::make_unique<TestShape>(*this);
    }

    Tuple NormalAtLocal(const Tuple& point, const Intersection& intersection) const override
    {
        return Tuple(0.f, 1.f, 0.f);
    }

    std::vector<Intersection> IntersectLocal(const Ray& ray) const override
    {
        return {};
    }

    BoundingBox GetBounds() const override
    {
        return BoundingBox(Point(-1.f, -1.f, -1.f), Point(1.f, 1.f, 1.f));
    }
};

TEST_CASE("Create an empty bounding box", "[bounds]")
{
    BoundingBox b;

    REQUIRE(b.GetMin() == Point(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
                                std::numeric_limits<float>::max()));
    REQUIRE(b.GetMax() == Point(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(),
                                -std::numeric_limits<float>::max()));
}

TEST_CASE("Creating abounding box with a volume", "[bounds]")
{
    BoundingBox b(Point(-1.f, -2.f, -3.f), Point(3.f, 2.f, 1.f));
    REQUIRE(b.GetMin() == Point(-1.f, -2.f, -3.f));
    REQUIRE(b.GetMax() == Point(3.f, 2.f, 1.f));
}

TEST_CASE("Adding points to an empty bounding box", "[bounds]")
{
    BoundingBox b;
    b.AddPoint(Point(-5.f, 2.f, 0.f));
    b.AddPoint(Point(7.f, 0.f, -3.f));
    REQUIRE(b.GetMin() == Point(-5.f, 0.f, -3.f));
    REQUIRE(b.GetMax() == Point(7.f, 2.f, 0.f));
}

TEST_CASE("Adding points to a non-empty bounding box", "[bounds]")
{
    BoundingBox b(Point(-5.f, 2.f, 0.f), Point(7.f, 0.f, -3.f));
    b.AddPoint(Point(-6.f, 3.f, 1.f));
    b.AddPoint(Point(8.f, -1.f, -4.f));
    REQUIRE(b.GetMin() == Point(-6.f, -1.f, -4.f));
    REQUIRE(b.GetMax() == Point(8.f, 3.f, 1.f));
}

TEST_CASE("Adding points that don't change the bounding box", "[bounds]")
{
    BoundingBox b(Point(-5.f, 2.f, 0.f), Point(7.f, 0.f, -3.f));
    b.AddPoint(Point(-4.f, 1.f, -1.f));
    b.AddPoint(Point(6.f, 0.5f, -2.f));

    REQUIRE(b.GetMin() == Point(-5.f, 0.f, -3.f));
    REQUIRE(b.GetMax() == Point(7.f, 2.f, 0.f));
}

TEST_CASE("A triangle has a bounding box", "[bounds]")
{
    Triangle t(Point(-3.f, 7.f, 2.f), Point(6.f, 2.f, -4.f), Point(2.f, -1.f, -1.f));
    BoundingBox b = t.GetBounds();
    REQUIRE(b.GetMin() == Point(-3.f, -1.f, -4.f));
    REQUIRE(b.GetMax() == Point(6.f, 7.f, 2.f));
}

TEST_CASE("Test shape has arbitrary bounds", "[bounds]")
{
    TestShape s("testshape");
    REQUIRE(s.GetBounds().GetMin() == Point(-1.f, -1.f, -1.f));
    REQUIRE(s.GetBounds().GetMax() == Point(1.f, 1.f, 1.f));
}

TEST_CASE("Adding one bounding box to another", "[bounds]")
{
    BoundingBox b1(Point(-5.f, -2.f, 0.f), Point(7.f, 4.f, 4.f));
    BoundingBox b2(Point(8.f, -7.f, -2.f), Point(14.f, 2.f, 8.f));
    b1 = b1 + b2;
    REQUIRE(b1.GetMin() == Point(-5.f, -7.f, -2.f));
    REQUIRE(b1.GetMax() == Point(14.f, 4.f, 8.f));
}
