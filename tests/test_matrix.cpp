#include <catch2/catch_test_macros.hpp>
#include "matrix.hpp"
#include "tuple.hpp"

TEST_CASE("Create matrix and check size", "[basic]")
{
    Matrix m = Matrix(4);
    REQUIRE(m.GetSize() == 4);
}

TEST_CASE("Create empty matrix and check default values", "[basic]")
{
    Matrix m = Matrix(4);
    for (int row = 0; row < m.GetSize(); ++row)
    {
        for (int col = 0; col < m.GetSize(); ++col)
        {
            REQUIRE(m.Get(col, row) == 0.f);
        }
    }
}

TEST_CASE("Get and set matrix values", "[basic]")
{
    Matrix m = Matrix(4);
    m.Set(0, 0, 1.f);
    m.Set(1, 1, 2.f);
    m.Set(2, 2, 3.f);
    m.Set(3, 3, 4.f);

    m.Set(3, 0, 5.f);

    REQUIRE(m.Get(0, 0) == 1.f);
    REQUIRE(m.Get(1, 1) == 2.f);
    REQUIRE(m.Get(2, 2) == 3.f);
    REQUIRE(m.Get(3, 3) == 4.f);
    REQUIRE(m.Get(3, 0) == 5.f);
}

TEST_CASE("Create a matrix from a vector of values and check them", "[basic]")
{
    std::vector<float> values = {
        1.f, 2.f, 3.f, 4.f,
        5.5f, 6.5f, 7.5f, 8.5f,
        9.f, 10.f, 11.f, 12.f,
        13.5f, 14.5f, 15.5f, 16.5f};
    Matrix m = Matrix(values);

    for (int row = 0; row < m.GetSize(); ++row)
    {
        for (int col = 0; col < m.GetSize(); ++col)
        {
            m.Set(col, row, values[row * m.GetSize() + col]);
        }
    }

    for (int row = 0; row < m.GetSize(); ++row)
    {
        for (int col = 0; col < m.GetSize(); ++col)
        {
            REQUIRE(m.Get(col, row) == values[row * m.GetSize() + col]);
        }
    }
}

TEST_CASE("Create a matrix with non-square vector of values should throw", "[basic]")
{
    std::vector<float> values = {
        1.f, 2.f, 3.f,
        4.f, 5.f, 6.f};
    REQUIRE_THROWS_AS(Matrix(values), std::invalid_argument);
}

TEST_CASE("Create a matrix with square vector of values should not throw", "[basic]")
{
    std::vector<float> values = {
        1.f, 2.f, 3.f, 4.f};
    REQUIRE_NOTHROW(Matrix(values));
}

TEST_CASE("Set matrix values outside of range should throw", "[basic]")
{
    Matrix m = Matrix(4);
    REQUIRE_THROWS_AS(m.Set(-1, -1, 1.f), std::out_of_range);
    REQUIRE_THROWS_AS(m.Set(4, 4, 1.f), std::out_of_range);
}

TEST_CASE("Get matrix values outside of range should throw", "[basic]")
{
    Matrix m = Matrix(4);
    REQUIRE_THROWS_AS(m.Get(-1, -1), std::out_of_range);
    REQUIRE_THROWS_AS(m.Get(4, 4), std::out_of_range);
}

TEST_CASE("Create a 2x2 matrix and check values", "[basic]")
{
    std::vector<float> values = {
        -3.f, 5.f,
        1.f, -2.f};
    Matrix m = Matrix(values);

    REQUIRE(m.Get(0, 0) == -3.f);
    REQUIRE(m.Get(1, 0) == 5.f);
    REQUIRE(m.Get(0, 1) == 1.f);
    REQUIRE(m.Get(1, 1) == -2.f);
}

TEST_CASE("Create 2 2x2 matrices and test for equality", "[basic]")
{
    std::vector<float> values1 = {
        -3.f, 5.f,
        1.f, -2.f};
    Matrix m1 = Matrix(values1);
    Matrix m2 = Matrix(values1);

    REQUIRE(m1 == m2);
}

TEST_CASE("Create 2 different 2x2 matrices and test for inequality", "[basic]")
{
    std::vector<float> values1 = {
        -3.f, 5.f,
        1.f, -2.f};
    std::vector<float> values2 = {
        -3.f, 5.f,
        1.f, -2.1f};
    Matrix m1 = Matrix(values1);
    Matrix m2 = Matrix(values2);

    REQUIRE(m1 != m2);
}

TEST_CASE("Create 2 different sized matrices and test for inequality", "[basic]")
{
    std::vector<float> values2x2 = {
        -3.f, 5.f,
        1.f, -2.f};
    std::vector<float> values3x3 = {
        -3.f, 5.f, 1.f,
        -2.f, 0.f, 0.f,
        0.f, 0.f, 0.f};
    Matrix m1 = Matrix(values2x2);
    Matrix m2 = Matrix(values3x3);

    REQUIRE(m1 != m2);
}

TEST_CASE("Multiply two 4x4 matrices", "[basic]")
{
    std::vector<float> values1 = {
        1.f, 2.f, 3.f, 4.f,
        5.f, 6.f, 7.f, 8.f,
        9.f, 8.f, 7.f, 6.f,
        5.f, 4.f, 3.f, 2.f};
    std::vector<float> values2 = {
        -2.f, 1.f, 2.f, 3.f,
        3.f, 2.f, 1.f, -1.f,
        4.f, 3.f, 6.f, 5.f,
        1.f, 2.f, 7.f, 8.f};
    Matrix m1 = Matrix(values1);
    Matrix m2 = Matrix(values2);
    Matrix result = m1 * m2;

    std::vector<float> expectedValues = {
        20.f, 22.f, 50.f, 48.f,
        44.f, 54.f, 114.f, 108.f,
        40.f, 58.f, 110.f, 102.f,
        16.f, 26.f, 46.f, 42.f};
    Matrix expected = Matrix(expectedValues);
    REQUIRE(result == expected);
}

TEST_CASE("Multiply a 2x2 matrix by a 3x3 matrix should throw", "[basic]")
{
    std::vector<float> values2x2 = {
        1.f, 2.f,
        3.f, 4.f};
    std::vector<float> values3x3 = {
        1.f, 2.f, 3.f,
        4.f, 5.f, 6.f,
        7.f, 8.f, 9.f};
    Matrix m1 = Matrix(values2x2);
    Matrix m2 = Matrix(values3x3);

    REQUIRE_THROWS_AS(m1 * m2, std::invalid_argument);
}

TEST_CASE("Set a 2x2 Matrix to the identity matrix and check for equality", "[basic]")
{
    Matrix identity(2);
    identity.SetIdentity();

    Matrix m = Matrix(2);
    m.Set(0, 0, 1.f);
    m.Set(1, 1, 1.f);

    REQUIRE(m == identity);
}

TEST_CASE("Multiply a 4x4 Matrix with the identity and check the result is the same matrix", "[basic]")
{
    Matrix identity(4);
    identity.SetIdentity();

    std::vector<float> values = {
        1.f, 4.f, 9.f, 2.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, -30.f, 1.f, 0.f,
        9.f, 0.f, 0.f, 1.f};
    Matrix m = Matrix(values);

    Matrix result = m * identity;
    REQUIRE(result == m);

    Matrix result2 = identity * m;
    REQUIRE(result2 == m);
}

TEST_CASE("Multiply a matrix by a tuple", "[basic]")
{
    std::vector<float> values = {
        1.f, 2.f, 3.f, 4.f,
        2.f, 4.f, 4.f, 2.f,
        8.f, 6.f, 4.f, 1.f,
        0.f, 0.f, 0.f, 1.f};
    Matrix m = Matrix(values);
    Tuple t{1.f, 2.f, 3.f, 1.f};
    Tuple result = m * t;
    REQUIRE(result == Tuple{18.f, 24.f, 33.f, 1.f});
}

TEST_CASE("Multiply a matrix by a tuple with wrong size should throw", "[basic]")
{
    std::vector<float> values = {
        1.f, 2.f, 3.f,
        4.f, 5.f, 6.f,
        7.f, 8.f, 9.f};
    Matrix m = Matrix(values);
    Tuple t{1.f, 2.f, 3.f, 1.f};

    REQUIRE_THROWS_AS(m * t, std::invalid_argument);
}

TEST_CASE("Multiply the identity matrix by a tuple should return the same tuple", "[basic]")
{
    Matrix identity(4);
    identity.SetIdentity();

    Tuple t{1.f, 2.f, 3.f, 1.f};
    Tuple result = identity * t;
    REQUIRE(result == t);
}

TEST_CASE("Transpose a matrix and check values", "[basic]")
{
    std::vector<float> values = {
        0.f, 9.f, 3.f, 0.f,
        9.f, 8.f, 0.f, 8.f,
        1.f, 8.f, 5.f, 3.f,
        0.f, 0.f, 5.f, 8.f};
    Matrix m = Matrix(values);

    std::vector<float> expectedValues = {
        0.f, 9.f, 1.f, 0.f,
        9.f, 8.f, 8.f, 0.f,
        3.f, 0.f, 5.f, 5.f,
        0.f, 8.f, 3.f, 8.f};
    Matrix expected = Matrix(expectedValues);

    Matrix result = m.Transpose();
    REQUIRE(result == expected);
}

TEST_CASE("Transpose the identity matrix should return the identity matrix", "[basic]")
{
    Matrix identity(4);
    identity.SetIdentity();

    Matrix result = identity.Transpose();
    REQUIRE(result == identity);
}

TEST_CASE("Determinant of a 2x2 matrix", "[basic]")
{
    std::vector<float> values = {
        1.f, 5.f,
        -3.f, 2.f};
    Matrix m = Matrix(values);
    float det = m.GetDeterminant();
    REQUIRE(AreEqual(det, 17.f));
}

TEST_CASE("A submatrix of a 3x3 matrix is a 2x2 matrix", "[basic]")
{
    std::vector<float> values = {
        1.f, 5.f, 0.f,
        -3.f, 2.f, 7.f,
        0.f, 6.f, -3.f};
    Matrix m = Matrix(values);
    Matrix sub = m.GetSubmatrix(0, 2);
    std::vector<float> expectedValues = {
        -3.f, 2.f,
        0.f, 6.f};
    Matrix expected = Matrix(expectedValues);
    REQUIRE(sub == expected);
}

TEST_CASE("A submatrix of a 4x4 matrix is a 3x3 matrix", "[basic]")
{
    std::vector<float> values = {
        -6.f, 1.f, 1.f, 6.f,
        -8.f, 5.f, 8.f, 6.f,
        -1.f, 0.f, 8.f, 2.f,
        -7.f, 1.f, -1.f, 1.f};
    Matrix m = Matrix(values);
    Matrix sub = m.GetSubmatrix(2, 1);
    std::vector<float> expectedValues = {
        -6.f, 1.f, 6.f,
        -8.f, 8.f, 6.f,
        -7.f, -1.f, 1.f};
    Matrix expected = Matrix(expectedValues);
    REQUIRE(sub == expected);
}

TEST_CASE("A submatrix with invalid row or column should throw", "[basic]")
{
    std::vector<float> values = {
        -6.f, 1.f, 1.f, 6.f,
        -8.f, 5.f, 8.f, 6.f,
        -1.f, 0.f, 8.f, 2.f,
        -7.f, 1.f, -1.f, 1.f};
    Matrix m = Matrix(values);
    REQUIRE_THROWS_AS(m.GetSubmatrix(-1, 0), std::out_of_range);
    REQUIRE_THROWS_AS(m.GetSubmatrix(0, -1), std::out_of_range);
    REQUIRE_THROWS_AS(m.GetSubmatrix(4, 0), std::out_of_range);
    REQUIRE_THROWS_AS(m.GetSubmatrix(0, 4), std::out_of_range);
}

TEST_CASE("Calculate the minor of a 3x3 matrix", "[basic]")
{
    std::vector<float> values = {
        3.f, 5.f, 0.f,
        2.f, -1.f, -7.f,
        6.f, -1.f, 5.f};
    Matrix m = Matrix(values);
    Matrix sub = m.GetSubmatrix(1, 0);
    float det = sub.GetDeterminant();
    REQUIRE(AreEqual(det, 25.f));
    // REQUIRE(AreEqual(m.GetMinor(1, 0), 25.f));
}