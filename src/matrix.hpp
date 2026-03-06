#pragma once
#include <vector>
#include "tuple.hpp"

class Matrix
{
public:
    Matrix(int size);
    Matrix(const std::vector<float> &values);

    int GetSize() const;
    float Get(int col, int row) const;
    void Set(int col, int row, float value);
    void SetIdentity();
    Matrix Transpose() const;
    float GetDeterminant() const;
    Matrix GetSubmatrix(int excludeRow, int excludeCol) const;

    bool operator==(const Matrix &other) const;
    bool operator!=(const Matrix &other) const;
    Matrix operator*(const Matrix &other) const;
    Tuple operator*(const Tuple &tuple) const;

private:
    int size; // Size of the matrix (e.g., 4 for a 4x4 matrix)
    std::vector<float> values;
};
