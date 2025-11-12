// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <cmath>

export module jpt.Matrix44;

import jpt.Concepts;
import jpt.Constants;
import jpt.Math;
import jpt.Quaternion;
import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Vector3;
import jpt.Vector4;

export namespace jpt
{
    /** Column-major */
    template<Numeric T>
    struct TMatrix44
    {
    public:
        Vector4<T> m[4];

    public:
        using NumericType = T;

        static consteval TMatrix44 Identity() { return TMatrix44(); }
        static consteval TMatrix44 Zero() { return TMatrix44(Vector4<T>::Zero(), Vector4<T>::Zero(), Vector4<T>::Zero(), Vector4<T>::Zero()); }

    public:
        constexpr TMatrix44();
        constexpr TMatrix44(const Vector4<T>& v0, const Vector4<T>& v1, const Vector4<T>& v2, const Vector4<T>& v3);
        constexpr TMatrix44(T m00, T m01, T m02, T m03,
                            T m10, T m11, T m12, T m13,
                            T m20, T m21, T m22, T m23,
                            T m30, T m31, T m32, T m33);

    public:
        [[nodiscard]] constexpr TMatrix44<T>  operator*(const TMatrix44<T>& rhs) const noexcept;
        [[nodiscard]] constexpr Vector4<T>    operator*(const Vector4<T>& rhs)   const noexcept;
        constexpr TMatrix44<T>& operator*=(const TMatrix44<T>& rhs) noexcept;

        [[nodiscard]] constexpr       Vector4<T>& operator[](size_t index) noexcept;
        [[nodiscard]] constexpr const Vector4<T>& operator[](size_t index) const noexcept;

    public:
        // Translation & Position
        [[nodiscard]] constexpr static TMatrix44<T> Translate(const Vector3<T>& v) noexcept;
        [[nodiscard]] constexpr Vector3<T> GetPosition() const noexcept;

        // Rotation & Orientation
        [[nodiscard]] constexpr static TMatrix44<T> RotateX(T radians) noexcept;
        [[nodiscard]] constexpr static TMatrix44<T> RotateY(T radians) noexcept;
        [[nodiscard]] constexpr static TMatrix44<T> RotateZ(T radians) noexcept;
        [[nodiscard]] constexpr static TMatrix44<T> FromEulerAngles(const Vector3<T>& eulerAngles) noexcept;
        [[nodiscard]] constexpr static TMatrix44<T> FromQuaternion(const TQuaternion<T>& quaternion) noexcept;
        [[nodiscard]] constexpr Vector3<T> CalcEulerAngles() const;    // Euler Angles in Radians

        // Scaling & Size
        [[nodiscard]] constexpr static TMatrix44<T> Scale(const Vector3<T>& v) noexcept;
        [[nodiscard]] constexpr Vector3<T> GetScale() const noexcept;

        /** Transpose. Swaps elements across the main diagonal. Used in checking orthogonality and normalizing the matrix. */
        [[nodiscard]] constexpr static TMatrix44<T> Transpose(const TMatrix44<T>& m) noexcept;

        /** Inverse matrix's behaviors. Undo */
        [[nodiscard]] constexpr static TMatrix44<T> Inverse(const TMatrix44<T>& m) noexcept;

        /** Creates an orthographic projection matrix */
        [[nodiscard]] constexpr static TMatrix44<T> Orthographic(T left, T right, T bottom, T top, T near, T far) noexcept;

        /** Creates a view matrix for a camera, defining how the world is oriented relative to the camera's position */
        [[nodiscard]] constexpr static TMatrix44<T> LookAt(const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up = Vector3<T>::Up()) noexcept;

        /** Converts 3D coordinates into 2D screen coordinates */
        [[nodiscard]] constexpr static TMatrix44<T> Perspective(T fov, T aspect, T near, T far) noexcept;

        /** Measures the volume of the parallelepiped spanned by the vectors of the matrix.    If determinant is 0, matrix is not invertible. */
        [[nodiscard]] constexpr T Determinant() const noexcept;

        /** @return true if matrix is orthogonal. Validates that a matrix only contains rotation (no scaling/shearing) */
        [[nodiscard]] constexpr bool IsOrthogonal() const noexcept;
    };

    // ------------------------------------------------------------------------------------------------
    // Non-Member Functions
    // ------------------------------------------------------------------------------------------------
    template<Numeric T>
    [[nodiscard]] constexpr bool operator==(const TMatrix44<T>& lhs, const TMatrix44<T>& rhs) noexcept
    {
        return lhs.m[0] == rhs.m[0] &&
               lhs.m[1] == rhs.m[1] &&
               lhs.m[2] == rhs.m[2] &&
               lhs.m[3] == rhs.m[3];
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector3<T> operator*(const TMatrix44<T>& matrix44, const Vector3<T>& vector3) noexcept
    {
        const Vector4<T> vector4 = Vector4<T>(vector3, static_cast<T>(1));
        const Vector4<T> result = matrix44 * vector4;
        return Vector3<T>(result.x, result.y, result.z);
    }

    template<Numeric T>
    [[nodiscard]] constexpr String ToString(const TMatrix44<T>& m) noexcept
    {
        return String::Format<256>("\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f", m[0][0], m[0][1], m[0][2], m[0][3],
                                                                                                                                       m[1][0], m[1][1], m[1][2], m[1][3],
                                                                                                                                       m[2][0], m[2][1], m[2][2], m[2][3],
                                                                                                                                       m[3][0], m[3][1], m[3][2], m[3][3]);
    }

    // ------------------------------------------------------------------------------------------------
    // Member Functions
    // ------------------------------------------------------------------------------------------------
    template<Numeric T>
    constexpr TMatrix44<T>::TMatrix44()
        : m{ { 1, 0, 0, 0 }, 
             { 0, 1, 0, 0 }, 
             { 0, 0, 1, 0 }, 
             { 0, 0, 0, 1 } }
    {
    }

    template<Numeric T>
    constexpr TMatrix44<T>::TMatrix44(const Vector4<T>& v0, const Vector4<T>& v1, const Vector4<T>& v2, const Vector4<T>& v3)
        : m{ v0, v1, v2, v3 }
    {
    }
    template<Numeric T>
    constexpr TMatrix44<T>::TMatrix44(T m00, T m01, T m02, T m03, 
                                    T m10, T m11, T m12, T m13, 
                                    T m20, T m21, T m22, T m23, 
                                    T m30, T m31, T m32, T m33)
        : m{ { m00, m01, m02, m03 },
             { m10, m11, m12, m13 },
             { m20, m21, m22, m23 },
             { m30, m31, m32, m33 } }
    {
    }

    template<Numeric T>
    constexpr TMatrix44<T> TMatrix44<T>::operator*(const TMatrix44<T>& rhs) const noexcept
    {
        TMatrix44<T> result;

        // Column Major Order
        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t j = 0; j < 4; ++j)
            {
                result.m[j][i] = m[0][i] * rhs.m[j][0] + 
                                 m[1][i] * rhs.m[j][1] + 
                                 m[2][i] * rhs.m[j][2] + 
                                 m[3][i] * rhs.m[j][3];
            }
        }

        return result;
    }

    template<Numeric T>
    constexpr Vector4<T> TMatrix44<T>::operator*(const Vector4<T>& rhs) const noexcept
    {
        Vector4<T> result;
        for (size_t i = 0; i < 4; ++i)
        {
            result[i] = m[0][i] * rhs[0] + 
                        m[1][i] * rhs[1] + 
                        m[2][i] * rhs[2] + 
                        m[3][i] * rhs[3];
        }
        return result;
    }

    template<Numeric T>
    constexpr TMatrix44<T>& TMatrix44<T>::operator*=(const TMatrix44<T>& rhs) noexcept
    {
        *this = *this * rhs;
        return *this;
    }

    template<Numeric T>
    constexpr Vector4<T>& TMatrix44<T>::operator[](size_t index) noexcept
    {
        return m[index];
    }

    template<Numeric T>
    constexpr const Vector4<T>& TMatrix44<T>::operator[](size_t index) const noexcept
    {
        return m[index];
    }

    template<Numeric T>
    constexpr TMatrix44<T> TMatrix44<T>::Translate(const Vector3<T>& v) noexcept
    {
        return TMatrix44<T>(  1,   0,   0, 0,
                              0,   1,   0, 0,
                              0,   0,   1, 0,
                            v.x, v.y, v.z, 1);
    }

    template<Numeric T>
    constexpr Vector3<T> TMatrix44<T>::GetPosition() const noexcept
    {
        return Vector3<T>(m[3].x, m[3].y, m[3].z);
    }

    template<Numeric T>
    constexpr TMatrix44<T> TMatrix44<T>::RotateX(T radians) noexcept
    {
        const T cos = Cos(radians);
        const T sin = Sin(radians);
        return TMatrix44<T>(1,   0,    0,  0,
                            0,  cos,  sin, 0,
                            0, -sin,  cos, 0,
                            0,    0,    0, 1);
    }

    template<Numeric T>
    constexpr TMatrix44<T> TMatrix44<T>::RotateY(T radians) noexcept
    {
        const T cos = Cos(radians);
        const T sin = Sin(radians);
        return TMatrix44<T>( cos, 0, -sin, 0,
                               0, 1,    0, 0,
                             sin, 0,  cos, 0,
                               0, 0,    0, 1);
    }

    template<Numeric T>
    constexpr TMatrix44<T> TMatrix44<T>::RotateZ(T radians) noexcept
    {
        const T cos = Cos(radians);
        const T sin = Sin(radians);
        return TMatrix44<T>( cos,  sin, 0, 0,
                            -sin,  cos, 0, 0,
                               0,    0, 1, 0,
                               0,    0, 0, 1);
    }

    template<Numeric T>
    constexpr TMatrix44<T> TMatrix44<T>::FromEulerAngles(const Vector3<T>& eulerAngles) noexcept
    {
        return RotateX(eulerAngles.x) * RotateY(eulerAngles.y) * RotateZ(eulerAngles.z);
    }
    
    template<Numeric T>
    constexpr TMatrix44<T> TMatrix44<T>::FromQuaternion(const TQuaternion<T>& quaternion) noexcept
    {
        const T xx = quaternion.x * quaternion.x;
        const T yy = quaternion.y * quaternion.y;
        const T zz = quaternion.z * quaternion.z;
        const T xy = quaternion.x * quaternion.y;
        const T xz = quaternion.x * quaternion.z;
        const T yz = quaternion.y * quaternion.z;
        const T wx = quaternion.w * quaternion.x;
        const T wy = quaternion.w * quaternion.y;
        const T wz = quaternion.w * quaternion.z;

        // Column-major order
        return TMatrix44<T>(1 - 2 * (yy + zz),     2 * (xy + wz),     2 * (xz - wy), 0,
                                2 * (xy - wz), 1 - 2 * (xx + zz),     2 * (yz + wx), 0,
                                2 * (xz + wy),     2 * (yz - wx), 1 - 2 * (xx + yy), 0,
                                            0,                 0,                 0, 1);
    }

    template<Numeric T>
    constexpr Vector3<T> TMatrix44<T>::CalcEulerAngles() const
    {
        // Extracting Euler Angles from a Rotation Matrix

        const Vector3<T> scale = GetScale();
        TMatrix44<T> rotation = *this;
        rotation[0] = Vector4<T>(m[0][0] / scale.x, m[0][1] / scale.x, m[0][2] / scale.x, 0);
        rotation[1] = Vector4<T>(m[1][0] / scale.y, m[1][1] / scale.y, m[1][2] / scale.y, 0);
        rotation[2] = Vector4<T>(m[2][0] / scale.z, m[2][1] / scale.z, m[2][2] / scale.z, 0);
        rotation[3] = Vector4<T>(0, 0, 0, 1);

        const T sy = Sqrt(rotation[0][0] * rotation[0][0] + rotation[1][0] * rotation[1][0]);
        if (sy > kEpsilon<T>)
        {
            const T x = -Atan2(rotation[2][1], rotation[2][2]);
            const T y = -Atan2(-rotation[2][0], sy);
            const T z = -Atan2(rotation[1][0], rotation[0][0]);
            return Vector3<T>(x, y, z);
        }
        // Gimbal lock
        else
        {
            JPT_ASSERT(false, "Gimbal lock detected");
            return Vector3<T>(0);
        }
    }

    template<Numeric T>
    constexpr TMatrix44<T> TMatrix44<T>::Scale(const Vector3<T>& v) noexcept
    {
        return TMatrix44<T>(v.x,  0,   0, 0,
                             0, v.y,   0, 0,
                             0,   0, v.z, 0,
                             0,   0,   0, 1);
    }

    template<Numeric T>
    constexpr Vector3<T> TMatrix44<T>::GetScale() const noexcept
    {
        const T x = Vector3<T>(m[0].x, m[0].y, m[0].z).Length();
        const T y = Vector3<T>(m[1].x, m[1].y, m[1].z).Length();
        const T z = Vector3<T>(m[2].x, m[2].y, m[2].z).Length();
        return Vector3<T>(x, y, z);
    }

    template<Numeric T>
    constexpr TMatrix44<T> TMatrix44<T>::Transpose(const TMatrix44<T>& m) noexcept
    {
        return TMatrix44<T>(m[0][0], m[1][0], m[2][0], m[3][0],
                            m[0][1], m[1][1], m[2][1], m[3][1],
                            m[0][2], m[1][2], m[2][2], m[3][2],
                            m[0][3], m[1][3], m[2][3], m[3][3]);
    }

    template<Numeric T>
    constexpr TMatrix44<T> TMatrix44<T>::Inverse(const TMatrix44<T>& m) noexcept
    {
        const T det = m.Determinant();
        if (AreValuesClose(det, static_cast<T>(0)))
        {
            return TMatrix44<T>::Identity();
        }

        const T invDet = 1 / det;

        const T a = m[0][0];
        const T b = m[0][1];
        const T c = m[0][2];
        const T d = m[0][3];
        const T e = m[1][0];
        const T f = m[1][1];
        const T g = m[1][2];
        const T h = m[1][3];
        const T i = m[2][0];
        const T j = m[2][1];
        const T k = m[2][2];
        const T l = m[2][3];
        const T q = m[3][0];
        const T n = m[3][1];
        const T o = m[3][2];
        const T p = m[3][3];

        TMatrix44<T> result;
        result.m[0][0] = (f * k * p + g * l * n + h * j * o - f * l * o - g * j * p - h * k * n) * invDet;
        result.m[0][1] = (b * l * o + c * j * p + d * k * n - b * k * p - c * l * n - d * j * o) * invDet;
        result.m[0][2] = (b * g * p + c * h * n + d * f * o - b * h * o - c * f * p - d * g * n) * invDet;
        result.m[0][3] = (b * h * k + c * f * l + d * g * j - b * g * l - c * h * j - d * f * k) * invDet;
        result.m[1][0] = (e * l * o + g * i * p + h * k * q - e * k * p - g * l * q - h * i * o) * invDet;
        result.m[1][1] = (a * k * p + c * l * q + d * i * o - a * l * o - c * i * p - d * k * q) * invDet;
        result.m[1][2] = (a * h * o + c * f * p + d * g * q - a * g * p - c * h * q - d * f * o) * invDet;
        result.m[1][3] = (a * g * l + c * h * q + d * f * k - a * h * k - c * f * l - d * g * q) * invDet;
        result.m[2][0] = (e * j * p + f * l * q + h * i * n - e * l * n - f * i * p - h * j * q) * invDet;
        result.m[2][1] = (a * l * n + b * i * p + d * j * q - a * j * p - b * l * q - d * i * n) * invDet;
        result.m[2][2] = (a * f * p + b * h * q + d * e * n - a * h * n - b * e * p - d * f * q) * invDet;
        result.m[2][3] = (a * h * j + b * e * l + d * f * i - a * f * l - b * h * i - d * e * j) * invDet;
        result.m[3][0] = (e * k * n + f * i * o + g * j * q - e * j * o - f * k * q - g * i * n) * invDet;
        result.m[3][1] = (a * j * o + b * k * q + c * i * n - a * k * n - b * i * o - c * j * q) * invDet;
        result.m[3][2] = (a * g * n + b * e * o + c * f * q - a * f * o - b * g * q - c * e * n) * invDet;
        result.m[3][3] = (a * f * k + b * g * q + c * e * j - a * g * j - b * e * k - c * f * q) * invDet;

        return result;
    }

    template<Numeric T>
    constexpr TMatrix44<T> TMatrix44<T>::Orthographic(T left, T right, T bottom, T top, T near, T far) noexcept
    {
        TMatrix44<T> result = TMatrix44<T>::Identity();

        const T width = right - left;
        const T height = top - bottom;
        const T depth = far - near;

        result.m[0][0] = static_cast<T>(2) / width;
        result.m[1][1] = static_cast<T>(2) / height;
        result.m[2][2] = -static_cast<T>(2) / depth;
        result.m[3][0] = -(right + left) / width;
        result.m[3][1] = -(top + bottom) / height;
        result.m[3][2] = -(far + near) / depth;

        return result;
    }

    template<Numeric T>
    constexpr TMatrix44<T> TMatrix44<T>::LookAt(const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up /* = Vector3<T>::Up()*/) noexcept
    {
        // Calcualte the forward vector
        const Vector3<T> forward = (center - eye).Normalized();

        // Calculate the right vector
        const Vector3<T> right = forward.Cross(up).Normalized();

        // Calculate the up vector
        const Vector3<T> newUp = right.Cross(forward);

        // Create the view matrix
        TMatrix44<T> result = TMatrix44<T>::Identity();
        result.m[0][0] = right.x;
        result.m[1][0] = right.y;
        result.m[2][0] = right.z;

        result.m[0][1] = newUp.x;
        result.m[1][1] = newUp.y;
        result.m[2][1] = newUp.z;

        result.m[0][2] = -forward.x;
        result.m[1][2] = -forward.y;
        result.m[2][2] = -forward.z;

        result.m[3][0] = -right.Dot(eye);
        result.m[3][1] = -newUp.Dot(eye);
        result.m[3][2] = forward.Dot(eye);

        return result;
    }

    template<Numeric T>
    constexpr TMatrix44<T> TMatrix44<T>::Perspective(T fov, T aspect, T zNear, T zFar) noexcept
    {
        const T tanHalfFovy = std::tan(fov / static_cast<T>(2));
        TMatrix44<T> result = TMatrix44<T>::Zero();

        result.m[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
        result.m[1][1] = static_cast<T>(1) / tanHalfFovy;
        result.m[2][2] = -(zFar + zNear) / (zFar - zNear);
        result.m[2][3] = -static_cast<T>(1);
        result.m[3][2] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);

        return result;
    }

    template<Numeric T>
    constexpr bool TMatrix44<T>::IsOrthogonal() const noexcept
    {
        return Inverse(*this) == Transpose(*this);
    }

    template<Numeric T>
    constexpr T TMatrix44<T>::Determinant() const noexcept
    {
        const T a = m[0][0];
        const T b = m[0][1];
        const T c = m[0][2];
        const T d = m[0][3];
        const T e = m[1][0];
        const T f = m[1][1];
        const T g = m[1][2];
        const T h = m[1][3];
        const T i = m[2][0];
        const T j = m[2][1];
        const T k = m[2][2];
        const T l = m[2][3];
        const T q = m[3][0];
        const T n = m[3][1];
        const T o = m[3][2];
        const T p = m[3][3];

        const T det = a * f * k * p + a * g * l * n + a * h * j * o +
                      b * e * l * p + b * g * i * p + b * h * k * n +
                      c * e * j * p + c * f * l * q + c * h * i * q +
                      d * e * k * n + d * f * i * o + d * g * j * q -
                      a * f * l * o - a * g * j * p - a * h * k * q -
                      b * e * k * o - b * g * l * q - b * h * i * p -
                      c * e * l * n - c * f * i * p - c * h * j * q -
                      d * e * j * n - d * f * k * q - d * g * i * l;

        return det;
    }
}

export using Matrix44  = jpt::TMatrix44<Precision>;
export using Matrix44f = jpt::TMatrix44<float32>;