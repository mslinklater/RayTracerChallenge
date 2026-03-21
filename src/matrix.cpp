#include "matrix.hpp"
#include "maths.hpp"

Matrix::Matrix(int size)
    : size(size), values(size * size, 0.f)
{
}

Matrix::Matrix(const std::vector<float> &values)
{
    int vectorSize = static_cast<int>(values.size());
    float sqrtSize = std::sqrt(vectorSize);
    if (sqrtSize != static_cast<int>(sqrtSize))
    {
        throw std::invalid_argument("The number of values must be a perfect square.");
    }

    size = static_cast<int>(sqrtSize);
    this->values = values;
}

int Matrix::GetSize() const
{
    return size;
}

float Matrix::Get(int row, int col) const
{
    if (col < 0 || col >= size || row < 0 || row >= size)
    {
        throw std::out_of_range("Column and row indices must be within the bounds of the matrix.");
    }
    return values[row * size + col];
}

void Matrix::Set(int row, int col, float value)
{
    if (col < 0 || col >= size || row < 0 || row >= size)
    {
        throw std::out_of_range("Column and row indices must be within the bounds of the matrix.");
    }
    values[row * size + col] = value;
}

bool Matrix::operator==(const Matrix &other) const
{
    if (size != other.size)
    {
        return false;
    }
    for (size_t i = 0; i < values.size(); ++i)
    {
        // if (values[i] != other.values[i])
        if (!AreEqual(values[i], other.values[i]))
        {
            return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &other) const
{
    return !(*this == other);
}

Matrix Matrix::operator*(const Matrix &other) const
{
    if (size != other.size)
    {
        throw std::invalid_argument("Matrices must be of the same size for multiplication.");
    }
    Matrix result(size);
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            float sum = 0.f;
            for (int k = 0; k < size; ++k)
            {
                sum += Get(row, k) * other.Get(k, col);
            }
            result.Set(row, col, sum);
        }
    }
    return result;
}

void Matrix::SetIdentity()
{
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            Set(row, col, (row == col) ? 1.f : 0.f);
        }
    }
}

Tuple Matrix::operator*(const Tuple &tuple) const
{
    if (size != 4)
    {
        throw std::invalid_argument("Matrix must be 4x4 to multiply with a tuple.");
    }
    float x = Get(0, 0) * tuple.x + Get(0, 1) * tuple.y + Get(0, 2) * tuple.z + Get(0, 3) * tuple.w;
    float y = Get(1, 0) * tuple.x + Get(1, 1) * tuple.y + Get(1, 2) * tuple.z + Get(1, 3) * tuple.w;
    float z = Get(2, 0) * tuple.x + Get(2, 1) * tuple.y + Get(2, 2) * tuple.z + Get(2, 3) * tuple.w;
    float w = Get(3, 0) * tuple.x + Get(3, 1) * tuple.y + Get(3, 2) * tuple.z + Get(3, 3) * tuple.w;
    return Tuple{x, y, z, w};
}

Matrix Matrix::Transpose() const
{
    Matrix result(size);
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            result.Set(row, col, Get(col, row));
        }
    }
    return result;
}

float Matrix::GetDeterminant() const
{
    float determinant = 0.f;
    if (size == 2)
    {
        determinant = Get(0, 0) * Get(1, 1) - Get(1, 0) * Get(0, 1);
    }
    else
    {
        for (int col = 0; col < size; ++col)
        {
            determinant += Get(0, col) * GetCofactor(0, col);
        }
    }
    return determinant;
}

Matrix Matrix::GetSubmatrix(int excludeRow, int excludeCol) const
{
    if (excludeRow < 0 || excludeRow >= size || excludeCol < 0 || excludeCol >= size)
    {
        throw std::out_of_range("excludedRow and excludedColumn indices must be within the bounds of the matrix.");
    }

    Matrix submatrix(size - 1);

    int subRow = 0;
    for (int row = 0; row < size; ++row)
    {
        if (row == excludeRow)
        {
            continue;
        }
        int subCol = 0;
        for (int col = 0; col < size; ++col)
        {
            if (col == excludeCol)
            {
                continue;
            }
            submatrix.Set(subRow, subCol, Get(row, col));
            ++subCol;
        }
        ++subRow;
    }
    return submatrix;
}

float Matrix::GetMinor(int row, int col) const
{
    Matrix sub = GetSubmatrix(row, col);
    return sub.GetDeterminant();
}

float Matrix::GetCofactor(int row, int col) const
{
    float minor = GetMinor(row, col);
    return ((row + col) % 2 == 0) ? minor : -minor;
}

Matrix Matrix::GetInverse() const
{
    float determinant = GetDeterminant();
    if (AreEqual(determinant, 0.f))
    {
        throw std::runtime_error("Matrix is not invertible.");
    }

    Matrix inverse(size);
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            float cofactor = GetCofactor(row, col);
            inverse.Set(col, row, cofactor / determinant); // Note the transpose here
        }
    }
    return inverse;
}

Matrix Matrix::CreateTranslation(float x, float y, float z)
{
    Matrix translation(4);
    translation.SetIdentity();
    translation.Set(0, 3, x);
    translation.Set(1, 3, y);
    translation.Set(2, 3, z);
    return translation;
}

Matrix Matrix::CreateScaling(float x, float y, float z)
{
    Matrix scaling(4);
    scaling.SetIdentity();
    scaling.Set(0, 0, x);
    scaling.Set(1, 1, y);
    scaling.Set(2, 2, z);
    return scaling;
}

Matrix Matrix::CreateRotationX(float angle)
{
    Matrix rotation(4);
    rotation.SetIdentity();
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    rotation.Set(1, 1, cosA);
    rotation.Set(1, 2, -sinA);
    rotation.Set(2, 1, sinA);
    rotation.Set(2, 2, cosA);
    return rotation;
}

Matrix Matrix::CreateRotationY(float angle)
{
    Matrix rotation(4);
    rotation.SetIdentity();
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    rotation.Set(0, 0, cosA);
    rotation.Set(0, 2, sinA);
    rotation.Set(2, 0, -sinA);
    rotation.Set(2, 2, cosA);
    return rotation;
}

Matrix Matrix::CreateRotationZ(float angle)
{
    Matrix rotation(4);
    rotation.SetIdentity();
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    rotation.Set(0, 0, cosA);
    rotation.Set(0, 1, -sinA);
    rotation.Set(1, 0, sinA);
    rotation.Set(1, 1, cosA);
    return rotation;
}

Matrix Matrix::CreateShearing(float xy, float xz, float yx, float yz, float zx, float zy)
{
    Matrix shearing(4);
    shearing.SetIdentity();
    shearing.Set(0, 1, xy);
    shearing.Set(0, 2, xz);
    shearing.Set(1, 0, yx);
    shearing.Set(1, 2, yz);
    shearing.Set(2, 0, zx);
    shearing.Set(2, 1, zy);
    return shearing;
}

Matrix Matrix::ViewTransform(const Tuple &from, const Tuple &to, const Tuple &up)
{
    Tuple forward = (to - from).Normalize();
    Tuple upNormalized = up.Normalize();
    Tuple left = forward ^ upNormalized;
    Tuple trueUp = left ^ forward;

    Matrix orientation(4);
    orientation.SetIdentity();
    orientation.Set(0, 0, left.x);
    orientation.Set(0, 1, left.y);
    orientation.Set(0, 2, left.z);
    orientation.Set(1, 0, trueUp.x);
    orientation.Set(1, 1, trueUp.y);
    orientation.Set(1, 2, trueUp.z);
    orientation.Set(2, 0, -forward.x);
    orientation.Set(2, 1, -forward.y);
    orientation.Set(2, 2, -forward.z);

    Matrix translation = Matrix::CreateTranslation(-from.x, -from.y, -from.z);
    return orientation * translation;
}