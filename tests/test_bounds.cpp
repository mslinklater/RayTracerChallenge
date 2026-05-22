#include "boundingbox.hpp"
#include "shapes/cube.hpp"
#include "shapes/cylinder.hpp"
#include "shapes/sphere.hpp"
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

TEST_CASE("Checking to see if a box contains a given point", "[bounds]")
{
    struct TestCase
    {
        std::string name;
        Tuple point;
        bool expectedResult;
    };

    std::vector<TestCase> cases = {
        {"1", Point(5.f,  -2.f, 0.f),  true },
        {"2", Point(11.f, 4.f,  7.f),  true },
        {"3", Point(8.f,  1.f,  3.f),  true },
        {"4", Point(3.f,  0.f,  3.f),  false},
        {"5", Point(8.f,  -4.f, 3.f),  false},
        {"6", Point(8.f,  1.f,  -1.f), false},
        {"7", Point(13.f, 1.f,  3.f),  false},
        {"8", Point(8.f,  5.f,  3.f),  false},
        {"9", Point(8.f,  1.f,  8.f),  false}
    };

    BoundingBox b(Point(5.f, -2.f, 0.f), Point(11.f, 4.f, 7.f));
    for (const auto& testCase : cases)
    {
        DYNAMIC_SECTION("point " << testCase.name)
        {
            REQUIRE(b.Contains(testCase.point) == testCase.expectedResult);
        }
    }
}

TEST_CASE("Checking to see if a box contains a given box", "[bounds]")
{
    struct TestCase
    {
        std::string name;
        BoundingBox box;
        bool expectedResult;
    };

    std::vector<TestCase> cases = {
        {"1", BoundingBox(Point(5.f, -2.f, 0.f),  Point(11.f, 4.f, 7.f)), true },
        {"2", BoundingBox(Point(6.f, -1.f, 1.f),  Point(10.f, 3.f, 6.f)), true },
        {"3", BoundingBox(Point(4.f, -3.f, -1.f), Point(10.f, 3.f, 6.f)), false},
        {"4", BoundingBox(Point(6.f, -1.f, 1.f),  Point(12.f, 5.f, 8.f)), false}
    };

    BoundingBox b(Point(5.f, -2.f, 0.f), Point(11.f, 4.f, 7.f));
    for (const auto& testCase : cases)
    {
        DYNAMIC_SECTION("box " << testCase.name)
        {
            REQUIRE(b.Contains(testCase.box) == testCase.expectedResult);
        }
    }
}

TEST_CASE("Transforming a bounding box", "[bounds]")
{
    BoundingBox b(Point(-1.f, -1.f, -1.f), Point(1.f, 1.f, 1.f));
    Matrix rotation = Matrix::CreateRotationX(M_PI / 4.f) * Matrix::CreateRotationY(M_PI / 4.f);
    BoundingBox rotated = b.Transform(rotation);
    REQUIRE(rotated.GetMin() == Point(-1.4142f, -1.7071f, -1.7071f));
    REQUIRE(rotated.GetMax() == Point(1.4142f, 1.7071f, 1.7071f));
}
