#include "matrix.hpp"

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

float Matrix::Get(int col, int row) const
{
    if (col < 0 || col >= size || row < 0 || row >= size)
    {
        throw std::out_of_range("Column and row indices must be within the bounds of the matrix.");
    }
    return values[row * size + col];
}

void Matrix::Set(int col, int row, float value)
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
        if (values[i] != other.values[i])
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
                sum += Get(k, row) * other.Get(col, k);
            }
            result.Set(col, row, sum);
        }
    }
    return result;
}

void Matrix::SetIdentity()
{
    for (int i = 0; i < size; ++i)
    {
        Set(i, i, 1.f);
    }
}

Tuple Matrix::operator*(const Tuple &tuple) const
{
    if (size != 4)
    {
        throw std::invalid_argument("Matrix must be 4x4 to multiply with a tuple.");
    }
    float x = Get(0, 0) * tuple.x + Get(1, 0) * tuple.y + Get(2, 0) * tuple.z + Get(3, 0) * tuple.w;
    float y = Get(0, 1) * tuple.x + Get(1, 1) * tuple.y + Get(2, 1) * tuple.z + Get(3, 1) * tuple.w;
    float z = Get(0, 2) * tuple.x + Get(1, 2) * tuple.y + Get(2, 2) * tuple.z + Get(3, 2) * tuple.w;
    float w = Get(0, 3) * tuple.x + Get(1, 3) * tuple.y + Get(2, 3) * tuple.z + Get(3, 3) * tuple.w;
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
    if (size == 2)
    {
        return Get(0, 0) * Get(1, 1) - Get(1, 0) * Get(0, 1);
    }
    float determinant = 0.f;
    //    for (int col = 0; col < size; ++col)
    //    {
    //        determinant += Get(col, 0) * Cofactor(0, col);
    //    }
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
            submatrix.Set(subCol, subRow, Get(col, row));
            ++subCol;
        }
        ++subRow;
    }
    return submatrix;
}