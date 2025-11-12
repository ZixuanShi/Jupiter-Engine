// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Matrix33;

import jpt.Concepts;
import jpt.Constants;
import jpt.TypeDefs;
import jpt.Math;
import jpt.Vector2;
import jpt.Vector3;
import jpt.String;
import jpt.Utilities;

export namespace jpt
{
    /** Column-major */
    template<Numeric T>
    struct TMatrix33
    {
    public:
        Vector3<T> m[3];

    public:
        static consteval TMatrix33 Identity() { return TMatrix33(); }

    public:
        constexpr TMatrix33();
        constexpr TMatrix33(const Vector3<T>& xAxis, const Vector3<T>& yAxis, const Vector3<T>& zAxis);
        constexpr TMatrix33(T m00, T m01, T m02,
                            T m10, T m11, T m12,
                            T m20, T m21, T m22);

    public:
        [[nodiscard]] constexpr TMatrix33<T>  operator*(const TMatrix33<T>& rhs) const noexcept;
        [[nodiscard]] constexpr Vector3<T>    operator*(const Vector3<T>& vector3) const noexcept;
        constexpr TMatrix33<T>& operator*=(const TMatrix33<T>& rhs) noexcept;

        [[nodiscard]] constexpr       Vector3<T>& operator[](size_t index)       noexcept;
        [[nodiscard]] constexpr const Vector3<T>& operator[](size_t index) const noexcept;

    public:
        // Translation & Position
        [[nodiscard]] constexpr static TMatrix33 Translate(Vector2<T> v) noexcept;

        // Rotation & Orientation
        [[nodiscard]] constexpr static TMatrix33 Rotate(T radians) noexcept;

        // Scaling & Size
        [[nodiscard]] constexpr static TMatrix33 Scale(Vector2<T> v) noexcept;

        /** Swaps elements across the main diagonal. Used in checking orthogonality and normalizing the matrix. */
        [[nodiscard]] constexpr static TMatrix33 Transpose(const TMatrix33& matrix) noexcept;

        /** Inverse matrix's behaviors. Undo */
        [[nodiscard]] constexpr static TMatrix33 Inverse(const TMatrix33& m) noexcept;

        /** Measures the volume of the parallelepiped spanned by the vectors of the matrix.    If determinant is 0, matrix is not invertible. */
        [[nodiscard]] constexpr T Determinant() const noexcept;

        /** @return true if matrix is orthogonal. Validates that a matrix only contains rotation (no scaling/shearing) */
        [[nodiscard]] constexpr bool IsOrthogonal() const noexcept;
    };

    // ------------------------------------------------------------------------------------------------
    // Non-Member Functions
    // ------------------------------------------------------------------------------------------------
    template<Numeric T>
    [[nodiscard]] constexpr bool operator==(const TMatrix33<T>& lhs, const TMatrix33<T>& rhs)
    {
        return lhs.m[0] == rhs.m[0] &&
               lhs.m[1] == rhs.m[1] &&
               lhs.m[2] == rhs.m[2];
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator*(const TMatrix33<T>& matrix33, Vector2<T>& vector2) noexcept
    {
        const Vector3<T> vector3 = Vector3<T>(vector2, static_cast<T>(1));
        const Vector3<T> result = matrix33 * vector3;
        return Vector2<T>(result.x, result.y);
    }

    template<Numeric T>
    [[nodiscard]] constexpr String ToString(const TMatrix33<T>& m)
    {
        return String::Format<128>("\n%.3f, %.3f, %.3f\n%.3f, %.3f, %.3f\n%.3f, %.3f, %.3f", m[0][0], m[0][1], m[0][2],
                                                                                             m[1][0], m[1][1], m[1][2],
                                                                                             m[2][0], m[2][1], m[2][2]);
    }

    // ------------------------------------------------------------------------------------------------
    // Member Functions
    // ------------------------------------------------------------------------------------------------
    template<Numeric T>
    constexpr TMatrix33<T>::TMatrix33()
        : m{ { 1, 0, 0 },
             { 0, 1, 0 },
             { 0, 0, 1 } }
    {
    }

    template<Numeric T>
    constexpr TMatrix33<T>::TMatrix33(const Vector3<T>& xAxis, const Vector3<T>& yAxis, const Vector3<T>& zAxis)
        : m{ xAxis, yAxis, zAxis }
    {
    }

    template<Numeric T>
    constexpr TMatrix33<T>::TMatrix33(T m00, T m01, T m02, 
                                      T m10, T m11, T m12, 
                                      T m20, T m21, T m22)
        : m{ { m00, m01, m02 },
             { m10, m11, m12 },
             { m20, m21, m22 } }
    {
    }

    template<Numeric T>
    constexpr TMatrix33<T> TMatrix33<T>::operator*(const TMatrix33<T>& rhs) const noexcept
    {
        TMatrix33<T> result;

        // Column Major Order
        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                result.m[j][i] = m[0][i] * rhs.m[j][0] + 
                                 m[1][i] * rhs.m[j][1] + 
                                 m[2][i] * rhs.m[j][2];
            }
        }

        return result;
    }

    template<Numeric T>
    constexpr TMatrix33<T>& TMatrix33<T>::operator*=(const TMatrix33<T>& rhs) noexcept
    {
        *this = *this * rhs;
        return *this;
    }

    template<Numeric T>
    constexpr Vector3<T> TMatrix33<T>::operator*(const Vector3<T>& v) const noexcept
    {
        Vector3<T> result;
        for (size_t i = 0; i < 3; ++i)
        {
            result[i] = m[0][i] * v[0] + 
                        m[1][i] * v[1] + 
                        m[2][i] * v[2];
        }
        return result;
    }

    template<Numeric T>
    constexpr Vector3<T>& TMatrix33<T>::operator[](size_t index) noexcept
    {
        return m[index];
    }

    template<Numeric T>
    constexpr const Vector3<T>& TMatrix33<T>::operator[](size_t index) const noexcept
    {
        return m[index];
    }

    template<Numeric T>
    constexpr TMatrix33<T> TMatrix33<T>::Translate(Vector2<T> v) noexcept
    {
        return TMatrix33<T>(1,   0, 0,
                            0,   1, 0,
                          v.x, v.y, 1);
    }

    template<Numeric T>
    constexpr TMatrix33<T> TMatrix33<T>::Rotate(T radians) noexcept
    {
        const T cos = Cos(radians);
        const T sin = Sin(radians);

        return TMatrix33<T>(cos,  sin, 0,
                           -sin,  cos, 0,
                             0,    0, 1);
    }

    template<Numeric T>
    constexpr TMatrix33<T> TMatrix33<T>::Scale(Vector2<T> v) noexcept
    {
        return TMatrix33<T>(v.x,   0, 0,
                              0, v.y, 0,
                              0,   0, 1);
    }

    template<Numeric T>
    constexpr TMatrix33<T> TMatrix33<T>::Transpose(const TMatrix33& matrix) noexcept
    {
        return TMatrix33(matrix.m[0][0], matrix.m[1][0], matrix.m[2][0],
                         matrix.m[0][1], matrix.m[1][1], matrix.m[2][1],
                         matrix.m[0][2], matrix.m[1][2], matrix.m[2][2]);
    }

    template<Numeric T>
    constexpr TMatrix33<T> TMatrix33<T>::Inverse(const TMatrix33& m) noexcept
    {
        const T det = m.Determinant();
        if (AreValuesClose(det, static_cast<T>(0)))
        {
            return TMatrix33<T>();
        }

        const T invDet = static_cast<T>(1) / det;

        TMatrix33<T> result;
        result.m[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
        result.m[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet;
        result.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;
        result.m[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet;
        result.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
        result.m[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * invDet;
        result.m[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
        result.m[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * invDet;
        result.m[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;

        return result;
    }

    template<Numeric T>
    constexpr T TMatrix33<T>::Determinant() const noexcept
    {
        return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
               m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
               m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    }

    template<Numeric T>
    constexpr bool TMatrix33<T>::IsOrthogonal() const noexcept
    {
        return Inverse(*this) == Transpose(*this);
    }
}

export using Matrix33  = jpt::TMatrix33<Precision>;
export using Matrix33f = jpt::TMatrix33<float32>;