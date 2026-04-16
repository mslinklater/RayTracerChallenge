#include <catch2/catch_test_macros.hpp>
#include "matrix.hpp"
#include "tuple.hpp"
#include "maths.hpp"
#include <limits>

TEST_CASE("Create matrix and check size", "[matrix]")
{
    Matrix m = Matrix(4);
    REQUIRE(m.GetSize() == 4);
}

TEST_CASE("Create empty matrix and check default values", "[matrix]")
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

TEST_CASE("Matrix with finite values is valid", "[matrix]")
{
    Matrix matrix({1.f, 2.f, 3.f, 4.f});

    REQUIRE(matrix.IsValid());
}

TEST_CASE("Matrix with infinite values is invalid", "[matrix]")
{
    Matrix matrix({1.f, std::numeric_limits<float>::infinity(), 3.f, 4.f});

    REQUIRE(!matrix.IsValid());
}

TEST_CASE("Matrix with NaN values is invalid", "[matrix]")
{
    Matrix matrix({1.f, std::numeric_limits<float>::quiet_NaN(), 3.f, 4.f});

    REQUIRE(!matrix.IsValid());
}

TEST_CASE("Get and set matrix values", "[matrix]")
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

TEST_CASE("Create a matrix from a vector of values and check them", "[matrix]")
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

TEST_CASE("Create a matrix with non-square vector of values should throw", "[matrix]")
{
    std::vector<float> values = {
        1.f, 2.f, 3.f,
        4.f, 5.f, 6.f};
    REQUIRE_THROWS_AS(Matrix(values), std::invalid_argument);
}

TEST_CASE("Create a matrix with square vector of values should not throw", "[matrix]")
{
    std::vector<float> values = {
        1.f, 2.f, 3.f, 4.f};
    REQUIRE_NOTHROW(Matrix(values));
}

TEST_CASE("Set matrix values outside of range should throw", "[matrix]")
{
    Matrix m = Matrix(4);
    REQUIRE_THROWS_AS(m.Set(-1, -1, 1.f), std::out_of_range);
    REQUIRE_THROWS_AS(m.Set(4, 4, 1.f), std::out_of_range);
}

TEST_CASE("Create a 2x2 matrix and check values", "[matrix]")
{
    std::vector<float> values = {
        -3.f, 5.f,
        1.f, -2.f};
    Matrix m = Matrix(values);

    REQUIRE(m.Get(0, 0) == -3.f);
    REQUIRE(m.Get(0, 1) == 5.f);
    REQUIRE(m.Get(1, 0) == 1.f);
    REQUIRE(m.Get(1, 1) == -2.f);
}

TEST_CASE("Create 2 2x2 matrices and test for equality", "[matrix]")
{
    std::vector<float> values1 = {
        -3.f, 5.f,
        1.f, -2.f};
    Matrix m1 = Matrix(values1);
    Matrix m2 = Matrix(values1);

    REQUIRE(m1 == m2);
}

TEST_CASE("Create 2 different 2x2 matrices and test for inequality", "[matrix]")
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

TEST_CASE("Create 2 different sized matrices and test for inequality", "[matrix]")
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

TEST_CASE("Multiply two 4x4 matrices", "[matrix]")
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

TEST_CASE("Multiply a 2x2 matrix by a 3x3 matrix should throw", "[matrix]")
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

TEST_CASE("Set a 2x2 Matrix to the identity matrix and check for equality", "[matrix]")
{
    Matrix identity(2);
    identity.SetIdentity();

    Matrix m = Matrix(2);
    m.Set(0, 0, 1.f);
    m.Set(1, 1, 1.f);

    REQUIRE(m == identity);
}

TEST_CASE("Multiply a 4x4 Matrix with the identity and check the result is the same matrix", "[matrix]")
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

TEST_CASE("Multiply a matrix by a tuple", "[matrix]")
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

TEST_CASE("Multiply a matrix by a tuple with wrong size should throw", "[matrix]")
{
    std::vector<float> values = {
        1.f, 2.f, 3.f,
        4.f, 5.f, 6.f,
        7.f, 8.f, 9.f};
    Matrix m = Matrix(values);
    Tuple t{1.f, 2.f, 3.f, 1.f};

    REQUIRE_THROWS_AS(m * t, std::invalid_argument);
}

TEST_CASE("Multiply the identity matrix by a tuple should return the same tuple", "[matrix]")
{
    Matrix identity(4);
    identity.SetIdentity();

    Tuple t{1.f, 2.f, 3.f, 1.f};
    Tuple result = identity * t;
    REQUIRE(result == t);
}

TEST_CASE("Transpose a matrix and check values", "[matrix]")
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

TEST_CASE("Transpose the identity matrix should return the identity matrix", "[matrix]")
{
    Matrix identity(4);
    identity.SetIdentity();

    Matrix result = identity.Transpose();
    REQUIRE(result == identity);
}

TEST_CASE("Determinant of a 2x2 matrix", "[matrix]")
{
    std::vector<float> values = {
        1.f, 5.f,
        -3.f, 2.f};
    Matrix m = Matrix(values);
    float det = m.GetDeterminant();
    REQUIRE(AreEqual(det, 17.f));
}

TEST_CASE("A submatrix of a 3x3 matrix is a 2x2 matrix", "[matrix]")
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

TEST_CASE("A submatrix of a 4x4 matrix is a 3x3 matrix", "[matrix]")
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

TEST_CASE("A submatrix with invalid row or column should throw", "[matrix]")
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

TEST_CASE("Calculate the minor of a 3x3 matrix", "[matrix]")
{
    std::vector<float> values = {
        3.f, 5.f, 0.f,
        2.f, -1.f, -7.f,
        6.f, -1.f, 5.f};
    Matrix m = Matrix(values);
    Matrix sub = m.GetSubmatrix(1, 0);
    float det = sub.GetDeterminant();
    REQUIRE(AreEqual(det, 25.f));
    REQUIRE(AreEqual(m.GetMinor(1, 0), 25.f));
}

TEST_CASE("Calculating the cofactor of a 3x3 matrix", "[matrix]")
{
    std::vector<float> values = {
        3.f, 5.f, 0.f,
        2.f, -1.f, -7.f,
        6.f, -1.f, 5.f};
    Matrix m = Matrix(values);
    REQUIRE(AreEqual(m.GetMinor(0, 0), -12.f));
    REQUIRE(AreEqual(m.GetCofactor(0, 0), -12.f));
    REQUIRE(AreEqual(m.GetMinor(1, 0), 25.f));
    REQUIRE(AreEqual(m.GetCofactor(1, 0), -25.f));
}

TEST_CASE("Calculating the determinant of a 3x3 matrix", "[matrix]")
{
    std::vector<float> values = {
        1.f, 2.f, 6.f,
        -5.f, 8.f, -4.f,
        2.f, 6.f, 4.f};
    Matrix m = Matrix(values);
    REQUIRE(AreEqual(m.GetCofactor(0, 0), 56.f));
    REQUIRE(AreEqual(m.GetCofactor(0, 1), 12.f));
    REQUIRE(AreEqual(m.GetCofactor(0, 2), -46.f));
    REQUIRE(AreEqual(m.GetDeterminant(), -196.f));
}

TEST_CASE("Calculating the determinant of a 4x4 matrix", "[matrix]")
{
    std::vector<float> values = {
        -2.f, -8.f, 3.f, 5.f,
        -3.f, 1.f, 7.f, 3.f,
        1.f, 2.f, -9.f, 6.f,
        -6.f, 7.f, 7.f, -9.f};
    Matrix m = Matrix(values);
    REQUIRE(AreEqual(m.GetCofactor(0, 0), 690.f));
    REQUIRE(AreEqual(m.GetCofactor(0, 1), 447.f));
    REQUIRE(AreEqual(m.GetCofactor(0, 2), 210.f));
    REQUIRE(AreEqual(m.GetCofactor(0, 3), 51.f));
    REQUIRE(AreEqual(m.GetDeterminant(), -4071.f));
}

TEST_CASE("Testing an invertible matrix for invertibility", "[matrix]")
{
    std::vector<float> values = {
        6.f, 4.f, 4.f, 4.f,
        5.f, 5.f, 7.f, 6.f,
        4.f, -9.f, 3.f, -7.f,
        9.f, 1.f, 7.f, -6.f};
    Matrix m = Matrix(values);
    REQUIRE(AreEqual(m.GetDeterminant(), -2120.f));
    REQUIRE(m.GetDeterminant() != 0.f);
}

TEST_CASE("Testing a non-invertible matrix for invertibility", "[matrix]")
{
    std::vector<float> values = {
        -4.f, 2.f, -2.f, -3.f,
        9.f, 6.f, 2.f, 6.f,
        0.f, -5.f, 1.f, -5.f,
        0.f, 0.f, 0.f, 0.f};
    Matrix m = Matrix(values);
    REQUIRE(AreEqual(m.GetDeterminant(), 0.f));
    REQUIRE(m.GetDeterminant() == 0.f);
}

TEST_CASE("Calculating the inverse of a non-invertible matrix throws", "[matrix]")
{
    std::vector<float> values = {
        -4.f, 2.f, -2.f, -3.f,
        9.f, 6.f, 2.f, 6.f,
        0.f, -5.f, 1.f, -5.f,
        0.f, 0.f, 0.f, 0.f};
    Matrix m = Matrix(values);

    REQUIRE_THROWS_AS(m.GetInverse(), std::runtime_error);
}

TEST_CASE("Calculating the inverse of a matrix", "[matrix]")
{
    std::vector<float> values = {
        -5.f, 2.f, 6.f, -8.f,
        1.f, -5.f, 1.f, 8.f,
        7.f, 7.f, -6.f, -7.f,
        1.f, -3.f, 7.f, 4.f};
    Matrix m = Matrix(values);
    Matrix inv = m.GetInverse();
    float det = m.GetDeterminant();
    REQUIRE(AreEqual(det, 532.f));
    REQUIRE(AreEqual(m.GetCofactor(2, 3), -160.f));
    REQUIRE(AreEqual(inv.Get(3, 2), -160.f / 532.f));
    REQUIRE(AreEqual(m.GetCofactor(3, 2), 105.f));
    REQUIRE(AreEqual(inv.Get(2, 3), 105.f / 532.f));

    std::vector<float> expectedValues = {
        0.21805f, 0.45113f, 0.24060f, -0.04511f,
        -0.80827f, -1.45677f, -0.44361f, 0.52068f,
        -0.07895f, -0.22368f, -0.05263f, 0.19737f,
        -0.52256f, -0.81391f, -0.30075f, 0.30639f};
    Matrix expected = Matrix(expectedValues);

    REQUIRE(inv == expected);
}

TEST_CASE("Calculating the inverse of another matrix", "[matrix]")
{
    std::vector<float> values = {
        8.f, -5.f, 9.f, 2.f,
        7.f, 5.f, 6.f, 1.f,
        -6.f, 0.f, 9.f, 6.f,
        -3.f, 0.f, -9.f, -4.f};
    Matrix m = Matrix(values);
    Matrix inv = m.GetInverse();

    std::vector<float> expectedValues = {
        -0.15385f, -0.15385f, -0.28205f, -0.53846f,
        -0.07692f, 0.12308f, 0.02564f, 0.03077f,
        0.35897f, 0.35897f, 0.43590f, 0.92308f,
        -0.69231f, -0.69231f, -0.76923f, -1.92308f};
    Matrix expected = Matrix(expectedValues);

    REQUIRE(inv == expected);
}

TEST_CASE("Calculate the inverse of a third matrix", "[matrix]")
{
    std::vector<float> values = {
        9.f, 3.f, 0.f, 9.f,
        -5.f, -2.f, -6.f, -3.f,
        -4.f, 9.f, 6.f, 4.f,
        -7.f, 6.f, 6.f, 2.f};
    Matrix m = Matrix(values);
    Matrix inv = m.GetInverse();

    std::vector<float> expectedValues = {
        -0.04074f, -0.07778f, 0.14444f, -0.22222f,
        -0.07778f, 0.03333f, 0.36667f, -0.33333f,
        -0.02901f, -0.14630f, -0.10926f, 0.12963f,
        0.17778f, 0.06667f, -0.26667f, 0.33333f};
    Matrix expected = Matrix(expectedValues);

    REQUIRE(inv == expected);
}
