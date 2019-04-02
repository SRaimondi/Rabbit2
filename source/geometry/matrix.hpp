//
// Created by Simon on 2019-04-02.
//

#ifndef RABBIT2_MATRIX_HPP
#define RABBIT2_MATRIX_HPP

#include "geometry.hpp"

#include <cmath>
#include <array>

namespace Rabbit
{
namespace Geometry
{

template <typename T>
class Matrix
{
public:
    Matrix() = default;

    Matrix(T m00, T m01, T m02, T m03,
           T m10, T m11, T m12, T m13,
           T m20, T m21, T m22, T m23,
           T m30, T m31, T m32, T m33) noexcept
        : elements{ m00, m01, m02, m03,
                    m10, m11, m12, m13,
                    m20, m21, m22, m23,
                    m30, m31, m32, m33 }
    {}

    Matrix(T d0, T d1, T d2, T d3) noexcept
        : Matrix(d0, 0, 0, 0,
                 0, d1, 0, 0,
                 0, 0, d2, 0,
                 0, 0, 0, d3)
    {}

    explicit Matrix(T diag) noexcept
        : Matrix(diag, diag, diag, diag)
    {}

    // Identity matrix
    static Matrix<T> Identity() noexcept
    {
        return Matrix<T>{ 1 };
    }

    T operator()(unsigned int row, unsigned int column) const noexcept
    {
        return elements[row][column];
    }

    T& operator()(unsigned int row, unsigned int column) noexcept
    {
        return elements[row][column];
    }

    const Point3 <T> operator()(const Point3 <T>& p) const noexcept
    {
        const T x{ elements[0][0] * p.x + elements[0][1] * p.y + elements[0][2] * p.z + elements[0][3] };
        const T y{ elements[1][0] * p.x + elements[1][1] * p.y + elements[1][2] * p.z + elements[1][3] };
        const T z{ elements[2][0] * p.x + elements[2][1] * p.y + elements[2][2] * p.z + elements[2][3] };
        const T inv_w{ T(1) / (elements[3][0] * p.x + elements[3][1] * p.y + elements[3][2] * p.z + elements[3][3]) };

        return { inv_w * x, inv_w * y, inv_w * z };
    }

    const Vector3 <T> operator()(const Vector3 <T>& v) const noexcept
    {
        return { elements[0][0] * v.x + elements[0][1] * v.y + elements[0][2] * v.z,
                 elements[1][0] * v.x + elements[1][1] * v.y + elements[1][2] * v.z,
                 elements[2][0] * v.x + elements[2][1] * v.y + elements[2][2] * v.z };
    }

    // The transform normal multiplies the matrix after transposing it
    const Vector3 <T> TransformNormal(const Vector3 <T>& n) const noexcept
    {
        return { elements[0][0] * n.x + elements[1][0] * n.y + elements[2][0] * n.z,
                 elements[0][1] * n.x + elements[1][1] * n.y + elements[2][1] * n.z,
                 elements[0][2] * n.x + elements[1][2] * n.y + elements[2][2] * n.z };
    }

private:
    // Matrix elements stored row-wise
    std::array<std::array<T, 4>, 4> elements;
};

// Compute determinant of a matrix
template <typename T>
inline T Determinant3x3(T m00, T m01, T m02,
                        T m10, T m11, T m12,
                        T m20, T m21, T m22) noexcept
{
    return m00 * (m11 * m22 - m21 * m12) - m01 * (m10 * m22 - m20 * m12) + m02 * (m10 * m21 - m20 * m11);
}

template <typename T>
inline T Determinant(const Matrix<T>& m) noexcept
{
    return m(0, 0) * Determinant3x3(m(1, 1), m(1, 2), m(1, 3),
                                    m(2, 1), m(2, 2), m(2, 3),
                                    m(3, 1), m(3, 2), m(3, 3)) -
           m(0, 1) * Determinant3x3(m(1, 0), m(1, 2), m(1, 3),
                                    m(2, 0), m(2, 2), m(2, 3),
                                    m(3, 0), m(3, 2), m(3, 3)) +
           m(0, 2) * Determinant3x3(m(1, 0), m(1, 1), m(1, 3),
                                    m(2, 0), m(2, 1), m(2, 3),
                                    m(3, 0), m(3, 1), m(3, 3)) -
           m(0, 3) * Determinant3x3(m(1, 0), m(1, 1), m(1, 2),
                                    m(2, 0), m(2, 1), m(2, 2),
                                    m(3, 0), m(3, 1), m(3, 2));
}

template <typename T>
Matrix<T> Inverse(const Matrix<T>& m)
{
    const T inv_determinant{ 1 / Determinant(m) };
    if (std::isinf(inv_determinant) || std::isnan(inv_determinant))
    {
        throw std::runtime_error("Invalid reciprocal determinant in matrix inversion\n");
    }

    Matrix<T> inverse;
    // First row
    inverse(0, 0) = inv_determinant * Determinant3x3(m(1, 1), m(1, 2), m(1, 3),
                                                     m(2, 1), m(2, 2), m(2, 3),
                                                     m(3, 1), m(3, 2), m(3, 3));
    inverse(0, 1) = -inv_determinant * Determinant3x3(m(0, 1), m(0, 2), m(0, 3),
                                                      m(2, 1), m(2, 2), m(2, 3),
                                                      m(3, 1), m(3, 2), m(3, 3));
    inverse(0, 2) = inv_determinant * Determinant3x3(m(0, 1), m(0, 2), m(0, 3),
                                                     m(1, 1), m(1, 2), m(1, 3),
                                                     m(3, 1), m(3, 2), m(3, 3));
    inverse(0, 3) = -inv_determinant * Determinant3x3(m(0, 1), m(0, 2), m(0, 3),
                                                      m(1, 1), m(1, 2), m(1, 3),
                                                      m(2, 1), m(2, 2), m(2, 3));

    // Second row
    inverse(1, 0) = -inv_determinant * Determinant3x3(m(1, 0), m(1, 2), m(1, 3),
                                                      m(2, 0), m(2, 2), m(2, 3),
                                                      m(3, 0), m(3, 2), m(3, 3));
    inverse(1, 1) = inv_determinant * Determinant3x3(m(0, 0), m(0, 2), m(0, 3),
                                                     m(2, 0), m(2, 2), m(2, 3),
                                                     m(3, 0), m(3, 2), m(3, 3));
    inverse(1, 2) = -inv_determinant * Determinant3x3(m(0, 0), m(0, 2), m(0, 3),
                                                      m(1, 0), m(1, 2), m(1, 3),
                                                      m(3, 0), m(3, 2), m(3, 3));
    inverse(1, 3) = inv_determinant * Determinant3x3(m(0, 0), m(0, 2), m(0, 3),
                                                     m(1, 0), m(1, 2), m(1, 3),
                                                     m(2, 0), m(2, 2), m(2, 3));

    // Third row
    inverse(2, 0) = inv_determinant * Determinant3x3(m(1, 0), m(1, 1), m(1, 3),
                                                     m(2, 0), m(2, 1), m(2, 3),
                                                     m(3, 0), m(3, 1), m(3, 3));
    inverse(2, 1) = -inv_determinant * Determinant3x3(m(0, 0), m(0, 1), m(0, 3),
                                                      m(2, 0), m(2, 1), m(2, 3),
                                                      m(3, 0), m(3, 1), m(3, 3));
    inverse(2, 2) = inv_determinant * Determinant3x3(m(0, 0), m(0, 1), m(0, 3),
                                                     m(1, 0), m(1, 1), m(1, 3),
                                                     m(3, 0), m(3, 1), m(3, 3));
    inverse(2, 3) = -inv_determinant * Determinant3x3(m(0, 0), m(0, 1), m(0, 3),
                                                      m(1, 0), m(1, 1), m(1, 3),
                                                      m(2, 0), m(2, 1), m(2, 3));

    // Fourth row
    inverse(3, 0) = -inv_determinant * Determinant3x3(m(1, 0), m(1, 1), m(1, 2),
                                                      m(2, 0), m(2, 1), m(2, 2),
                                                      m(3, 0), m(3, 1), m(3, 2));
    inverse(3, 1) = inv_determinant * Determinant3x3(m(0, 0), m(0, 1), m(0, 2),
                                                     m(2, 0), m(2, 1), m(2, 2),
                                                     m(3, 0), m(3, 1), m(3, 2));
    inverse(3, 2) = -inv_determinant * Determinant3x3(m(0, 0), m(0, 1), m(0, 2),
                                                      m(1, 0), m(1, 1), m(1, 2),
                                                      m(3, 0), m(3, 1), m(3, 2));
    inverse(3, 3) = inv_determinant * Determinant3x3(m(0, 0), m(0, 1), m(0, 2),
                                                     m(1, 0), m(1, 1), m(1, 2),
                                                     m(2, 0), m(2, 1), m(2, 2));

    return inverse;
}

template <typename T>
inline const Matrix<T> Transpose(const Matrix<T>& m) noexcept
{
    return { m(0, 0), m(1, 0), m(2, 0), m(3, 0),
             m(0, 1), m(1, 1), m(2, 1), m(3, 1),
             m(0, 2), m(1, 2), m(2, 2), m(3, 2),
             m(0, 3), m(1, 3), m(2, 3), m(3, 3) };
}

template <typename T>
inline const Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs) noexcept
{
    Matrix<T> product;

    for (unsigned int row = 0; row != 4; row++)
    {
        for (unsigned int column = 0; column != 4; column++)
        {
            product(row, column) = lhs(row, 0) * rhs(0, column) +
                lhs(row, 1) * rhs(1, column) +
                lhs(row, 2) * rhs(2, column) +
                lhs(row, 3) * rhs(3, column);
        }
    }

    return product;
}

// Create look at matrix
template <typename T>
const Matrix<T> LookAt(const Point3 <T>& eye, const Point3 <T>& at, const Vector3 <T>& up) noexcept
{
    // Compute local base
    const Vector3f w{ Normalize(eye - at) };
    const Vector3f u{ Normalize(Cross(up, w)) };
    const Vector3f v{ Cross(w, u) };

    return { u.x, v.x, w.x, eye.x,
             u.y, v.y, w.y, eye.y,
             u.z, v.z, w.z, eye.z,
             0, 0, 0, 1 };
}

using Matrixf = Matrix<float>;

} // Geometry namespace
} // Rabbit namespace

#endif //RABBIT2_MATRIX_HPP
