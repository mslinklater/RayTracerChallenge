#include <catch2/catch_test_macros.hpp>
#include "shape.hpp"
#include "matrix.hpp"

TEST_CASE("The default transformation", "[shapes]")
{
    Shape s("shape");
    Matrix expected(4);
    expected.SetIdentity();
    REQUIRE(s.GetTransform() == expected);
}

TEST_CASE("Assigning a transformation", "[shapes]")
{
    Shape s("shape");
    Matrix t = Matrix::CreateTranslation(2.f, 3.f, 4.f);
    s.SetTransform(t);
    Matrix expected(4);
    expected = Matrix::CreateTranslation(2.f, 3.f, 4.f);
    REQUIRE(s.GetTransform() == expected);
}

TEST_CASE("A shape has the default material", "[shapes]")
{
    Shape s("sphere");
    Material expected;
    REQUIRE(s.GetMaterial() == expected);
}

TEST_CASE("A shape may be assigned a material", "[shapes]")
{
    Shape s("sphere");
    Material m;
    m.SetAmbient(1.f);
    s.SetMaterial(m);
    Material expected;
    expected.SetAmbient(1.f);
    REQUIRE(s.GetMaterial() == expected);
}
