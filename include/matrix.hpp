#pragma once
#include <vector>
#include "tuple.hpp"

class Matrix
{
public:
    Matrix(int size);
    Matrix(const std::vector<float> &values);

    int GetSize() const;
    float Get(int row, int col) const;
    void Set(int row, int col, float value);
    void SetIdentity();
    Matrix Transpose() const;
    float GetDeterminant() const;
    Matrix GetSubmatrix(int excludeRow, int excludeCol) const;
    float GetMinor(int row, int col) const;
    float GetCofactor(int row, int col) const;
    Matrix GetInverse() const;

    bool operator==(const Matrix &other) const;
    bool operator!=(const Matrix &other) const;
    Matrix operator*(const Matrix &other) const;
    Tuple operator*(const Tuple &tuple) const;

    static Matrix CreateTranslation(float x, float y, float z);
    static Matrix CreateScaling(float x, float y, float z);
    static Matrix CreateRotationX(float angle);
    static Matrix CreateRotationY(float angle);
    static Matrix CreateRotationZ(float angle);
    static Matrix CreateShearing(float xy, float xz, float yx, float yz, float zx, float zy);

private:
    int size; // Size of the matrix (e.g., 4 for a 4x4 matrix)
    std::vector<float> values;
};
