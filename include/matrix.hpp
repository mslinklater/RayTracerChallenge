#pragma once
#include "tuple.hpp"
#include <vector>

/**
 * @brief A square NxN matrix supporting common linear-algebra operations.
 *
 * Used throughout the renderer for all spatial transformations (translation,
 * rotation, scaling, shearing) and the view transform.
 */
class Matrix
{
  public:
    /**
     * @brief Constructs an uninitialised matrix of the given side length.
     * @param size Side length (e.g., 4 for a 4×4 matrix).
     */
    Matrix(int size);

    /**
     * @brief Constructs a matrix from a flat row-major list of values.
     * @param values Flat list of @c size*size floats in row-major order.
     */
    Matrix(const std::vector<float> &values);

    /** @brief Returns the side length of the matrix. */
    int GetSize() const;

    /**
     * @brief Returns the element at (@p row, @p col).
     * @param row Zero-based row index.
     * @param col Zero-based column index.
     */
    float Get(int row, int col) const;

    /**
     * @brief Sets the element at (@p row, @p col) to @p value.
     * @param row Zero-based row index.
     * @param col Zero-based column index.
     * @param value The value to store.
     */
    void Set(int row, int col, float value);

    /** @brief Sets this matrix to the identity matrix. */
    void SetIdentity();

    /** @brief Returns the transpose of this matrix. */
    Matrix Transpose() const;

    /** @brief Returns the determinant of this matrix. */
    float GetDeterminant() const;

    /**
     * @brief Returns the submatrix formed by deleting the given row and column.
     * @param excludeRow Zero-based row to remove.
     * @param excludeCol Zero-based column to remove.
     */
    Matrix GetSubmatrix(int excludeRow, int excludeCol) const;

    /**
     * @brief Returns the minor of element (@p row, @p col).
     *
     * The minor is the determinant of the submatrix obtained by removing
     * @p row and @p col.
     */
    float GetMinor(int row, int col) const;

    /**
     * @brief Returns the cofactor of element (@p row, @p col).
     *
     * Equal to the minor with a sign flip when (row + col) is odd.
     */
    float GetCofactor(int row, int col) const;

    /** @brief Returns the inverse of this matrix. */
    Matrix GetInverse() const;
    /** @bried Returns true if all the matrix elements are finite values. */
    bool IsValid() const;

    /** @brief Equality comparison using floating-point tolerance. */
    bool operator==(const Matrix &other) const;
    /** @brief Inequality comparison using floating-point tolerance. */
    bool operator!=(const Matrix &other) const;
    /** @brief Matrix multiplication. */
    Matrix operator*(const Matrix &other) const;
    /** @brief Transforms a tuple by this matrix. */
    Tuple operator*(const Tuple &tuple) const;

    /**
     * @brief Creates a 4×4 translation matrix.
     * @param x Translation along X.
     * @param y Translation along Y.
     * @param z Translation along Z.
     */
    static Matrix CreateTranslation(float x, float y, float z);

    /**
     * @brief Creates a 4×4 scaling matrix.
     * @param x Scale factor along X.
     * @param y Scale factor along Y.
     * @param z Scale factor along Z.
     */
    static Matrix CreateScaling(float x, float y, float z);

    /**
     * @brief Creates a 4×4 rotation matrix around the X axis.
     * @param angle Rotation angle in radians.
     */
    static Matrix CreateRotationX(float angle);

    /**
     * @brief Creates a 4×4 rotation matrix around the Y axis.
     * @param angle Rotation angle in radians.
     */
    static Matrix CreateRotationY(float angle);

    /**
     * @brief Creates a 4×4 rotation matrix around the Z axis.
     * @param angle Rotation angle in radians.
     */
    static Matrix CreateRotationZ(float angle);

    /**
     * @brief Creates a 4×4 shearing (skew) matrix.
     * @param xy X moves in proportion to Y.
     * @param xz X moves in proportion to Z.
     * @param yx Y moves in proportion to X.
     * @param yz Y moves in proportion to Z.
     * @param zx Z moves in proportion to X.
     * @param zy Z moves in proportion to Y.
     */
    static Matrix CreateShearing(float xy, float xz, float yx, float yz, float zx, float zy);

    /**
     * @brief Creates a view transform matrix for a camera.
     * @param from The position of the camera (eye point).
     * @param to   The point the camera is looking at.
     * @param up   The approximate "up" direction.
     * @return A matrix that transforms world-space coordinates into eye space.
     */
    static Matrix ViewTransform(const Tuple &from, const Tuple &to, const Tuple &up);

  private:
    int size;                  ///< Side length of the matrix (e.g., 4 for a 4×4 matrix).
    std::vector<float> values; ///< Flat row-major storage of matrix elements.
};
