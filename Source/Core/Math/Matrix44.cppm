// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Matrix44;

import jpt.Assert;
import jpt.Concepts;
import jpt.Math;
import jpt.TypeDefs;
import jpt.Vector3;
import jpt.Vector4;

export namespace jpt
{
    /** Column-major, indexed m[column][row], applied as M * v. That is also MSL's float4x4
        layout and GLSL's layout(column_major), so an instance uploads to a GPU buffer by
        straight copy -- no transpose at the seam. */
    template<Numeric T>
    struct Matrix44
    {
        Vector4<T> m[4];

        constexpr Matrix44()
            : m{ { 1, 0, 0, 0 },
                 { 0, 1, 0, 0 },
                 { 0, 0, 1, 0 },
                 { 0, 0, 0, 1 } }
        {
        }

        constexpr Matrix44(const Vector4<T>& c0, const Vector4<T>& c1, const Vector4<T>& c2, const Vector4<T>& c3)
            : m{ c0, c1, c2, c3 }
        {
        }

        static consteval Matrix44 Identity() { return Matrix44(); }
        static consteval Matrix44 Zero()
        {
            return Matrix44(Vector4<T>::Zero(), Vector4<T>::Zero(), Vector4<T>::Zero(), Vector4<T>::Zero());
        }

        [[nodiscard]] constexpr Vector3<T> GetTranslation() const { return m[3].XYZ(); }

        // ------------------------------------------------------------------------------------
        // Transforms
        // ------------------------------------------------------------------------------------
        [[nodiscard]] static constexpr Matrix44 Translate(const Vector3<T>& v)
        {
            Matrix44 result;
            result.m[3] = Vector4<T>(v, static_cast<T>(1));
            return result;
        }

        [[nodiscard]] static constexpr Matrix44 Scale(const Vector3<T>& v)
        {
            Matrix44 result;
            result.m[0].x = v.x;
            result.m[1].y = v.y;
            result.m[2].z = v.z;
            return result;
        }

        // Right-handed rotations: positive angle turns counter-clockwise looking down the axis
        // toward the origin. This is what every standard derivation assumes.
        [[nodiscard]] static Matrix44 RotateX(T radians)
        {
            const T c = Cos(radians);
            const T s = Sin(radians);

            Matrix44 result;
            result.m[1] = Vector4<T>(0,  c, s, 0);
            result.m[2] = Vector4<T>(0, -s, c, 0);
            return result;
        }

        [[nodiscard]] static Matrix44 RotateY(T radians)
        {
            const T c = Cos(radians);
            const T s = Sin(radians);

            Matrix44 result;
            result.m[0] = Vector4<T>(c, 0, -s, 0);
            result.m[2] = Vector4<T>(s, 0,  c, 0);
            return result;
        }

        [[nodiscard]] static Matrix44 RotateZ(T radians)
        {
            const T c = Cos(radians);
            const T s = Sin(radians);

            Matrix44 result;
            result.m[0] = Vector4<T>( c, s, 0, 0);
            result.m[1] = Vector4<T>(-s, c, 0, 0);
            return result;
        }

        /** X * Y * Z, so Z is applied to the vector first. */
        [[nodiscard]] static Matrix44 FromEulerAngles(const Vector3<T>& radians)
        {
            return RotateX(radians.x) * RotateY(radians.y) * RotateZ(radians.z);
        }

        [[nodiscard]] static constexpr Matrix44 Transpose(const Matrix44& mat)
        {
            return Matrix44(Vector4<T>(mat.m[0].x, mat.m[1].x, mat.m[2].x, mat.m[3].x),
                            Vector4<T>(mat.m[0].y, mat.m[1].y, mat.m[2].y, mat.m[3].y),
                            Vector4<T>(mat.m[0].z, mat.m[1].z, mat.m[2].z, mat.m[3].z),
                            Vector4<T>(mat.m[0].w, mat.m[1].w, mat.m[2].w, mat.m[3].w));
        }

        /** Returns Identity for a singular matrix, but says so first -- legacy returned it
            silently, which turns a broken transform into a subtly wrong one. */
        [[nodiscard]] static Matrix44 Inverse(const Matrix44& mat) requires Floating<T>;

        // ------------------------------------------------------------------------------------
        // Camera and projection
        // ------------------------------------------------------------------------------------

        /** Right-handed: the camera looks down -Z in view space, matching Vector3::Forward(). */
        [[nodiscard]] static Matrix44 LookAt(const Vector3<T>& eye,
                                             const Vector3<T>& center,
                                             const Vector3<T>& up = Vector3<T>::Up()) requires Floating<T>
        {
            const Vector3<T> forward = (center - eye).Normalized();
            const Vector3<T> right   = forward.Cross(up).Normalized();
            const Vector3<T> newUp   = right.Cross(forward);

            return Matrix44(Vector4<T>(right.x, newUp.x, -forward.x, 0),
                            Vector4<T>(right.y, newUp.y, -forward.y, 0),
                            Vector4<T>(right.z, newUp.z, -forward.z, 0),
                            Vector4<T>(-right.Dot(eye), -newUp.Dot(eye), forward.Dot(eye), 1));
        }

        /** Right-handed, depth mapped to **[0, 1]** -- what Metal, Vulkan and D3D all clip
            against. Legacy emitted the OpenGL [-1, 1] form, which silently discards everything
            from the near plane to the middle of the frustum.

            No y-flip here: Metal's NDC is +Y up and Vulkan's is +Y down, so that correction
            belongs to whichever backend needs it, not to the shared matrix.

            @param fovY  Vertical field of view, in radians. */
        [[nodiscard]] static Matrix44 Perspective(T fovY, T aspect, T zNear, T zFar) requires Floating<T>
        {
            const T tanHalfFovY = Tan(fovY / static_cast<T>(2));

            Matrix44 result = Zero();
            result.m[0].x = static_cast<T>(1) / (aspect * tanHalfFovY);
            result.m[1].y = static_cast<T>(1) / tanHalfFovY;
            result.m[2].z = zFar / (zNear - zFar);
            result.m[2].w = static_cast<T>(-1);
            result.m[3].z = (zFar * zNear) / (zNear - zFar);
            return result;
        }

        // ------------------------------------------------------------------------------------
        // Operators
        // ------------------------------------------------------------------------------------
        [[nodiscard]] constexpr Matrix44 operator*(const Matrix44& rhs) const
        {
            Matrix44 result;
            for (usize col = 0; col < 4; ++col)
            {
                for (usize row = 0; row < 4; ++row)
                {
                    result.m[col][row] = m[0][row] * rhs.m[col][0] +
                                         m[1][row] * rhs.m[col][1] +
                                         m[2][row] * rhs.m[col][2] +
                                         m[3][row] * rhs.m[col][3];
                }
            }
            return result;
        }

        [[nodiscard]] constexpr Vector4<T> operator*(const Vector4<T>& rhs) const
        {
            Vector4<T> result;
            for (usize row = 0; row < 4; ++row)
            {
                result[row] = m[0][row] * rhs[0] +
                              m[1][row] * rhs[1] +
                              m[2][row] * rhs[2] +
                              m[3][row] * rhs[3];
            }
            return result;
        }

        constexpr Matrix44& operator*=(const Matrix44& rhs) { *this = *this * rhs; return *this; }

        constexpr bool operator==(const Matrix44& rhs) const = default;
    };

    /** Treats the vector as a position (w = 1) and drops w after the transform. */
    template<Numeric T>
    [[nodiscard]] constexpr Vector3<T> operator*(const Matrix44<T>& mat, const Vector3<T>& v)
    {
        return (mat * Vector4<T>(v, static_cast<T>(1))).XYZ();
    }

    template<Numeric T>
    Matrix44<T> Matrix44<T>::Inverse(const Matrix44& mat) requires Floating<T>
    {
        // Flattened column-major, so n[col * 4 + row] -- the layout the standard cofactor
        // expansion below is written against.
        T n[16];
        for (usize col = 0; col < 4; ++col)
        {
            for (usize row = 0; row < 4; ++row)
            {
                n[col * 4 + row] = mat.m[col][row];
            }
        }

        T inv[16];
        inv[0]  =  n[5]*n[10]*n[15] - n[5]*n[11]*n[14] - n[9]*n[6]*n[15] + n[9]*n[7]*n[14] + n[13]*n[6]*n[11] - n[13]*n[7]*n[10];
        inv[4]  = -n[4]*n[10]*n[15] + n[4]*n[11]*n[14] + n[8]*n[6]*n[15] - n[8]*n[7]*n[14] - n[12]*n[6]*n[11] + n[12]*n[7]*n[10];
        inv[8]  =  n[4]*n[9]*n[15]  - n[4]*n[11]*n[13] - n[8]*n[5]*n[15] + n[8]*n[7]*n[13] + n[12]*n[5]*n[11] - n[12]*n[7]*n[9];
        inv[12] = -n[4]*n[9]*n[14]  + n[4]*n[10]*n[13] + n[8]*n[5]*n[14] - n[8]*n[6]*n[13] - n[12]*n[5]*n[10] + n[12]*n[6]*n[9];
        inv[1]  = -n[1]*n[10]*n[15] + n[1]*n[11]*n[14] + n[9]*n[2]*n[15] - n[9]*n[3]*n[14] - n[13]*n[2]*n[11] + n[13]*n[3]*n[10];
        inv[5]  =  n[0]*n[10]*n[15] - n[0]*n[11]*n[14] - n[8]*n[2]*n[15] + n[8]*n[3]*n[14] + n[12]*n[2]*n[11] - n[12]*n[3]*n[10];
        inv[9]  = -n[0]*n[9]*n[15]  + n[0]*n[11]*n[13] + n[8]*n[1]*n[15] - n[8]*n[3]*n[13] - n[12]*n[1]*n[11] + n[12]*n[3]*n[9];
        inv[13] =  n[0]*n[9]*n[14]  - n[0]*n[10]*n[13] - n[8]*n[1]*n[14] + n[8]*n[2]*n[13] + n[12]*n[1]*n[10] - n[12]*n[2]*n[9];
        inv[2]  =  n[1]*n[6]*n[15]  - n[1]*n[7]*n[14]  - n[5]*n[2]*n[15] + n[5]*n[3]*n[14] + n[13]*n[2]*n[7]  - n[13]*n[3]*n[6];
        inv[6]  = -n[0]*n[6]*n[15]  + n[0]*n[7]*n[14]  + n[4]*n[2]*n[15] - n[4]*n[3]*n[14] - n[12]*n[2]*n[7]  + n[12]*n[3]*n[6];
        inv[10] =  n[0]*n[5]*n[15]  - n[0]*n[7]*n[13]  - n[4]*n[1]*n[15] + n[4]*n[3]*n[13] + n[12]*n[1]*n[7]  - n[12]*n[3]*n[5];
        inv[14] = -n[0]*n[5]*n[14]  + n[0]*n[6]*n[13]  + n[4]*n[1]*n[14] - n[4]*n[2]*n[13] - n[12]*n[1]*n[6]  + n[12]*n[2]*n[5];
        inv[3]  = -n[1]*n[6]*n[11]  + n[1]*n[7]*n[10]  + n[5]*n[2]*n[11] - n[5]*n[3]*n[10] - n[9]*n[2]*n[7]   + n[9]*n[3]*n[6];
        inv[7]  =  n[0]*n[6]*n[11]  - n[0]*n[7]*n[10]  - n[4]*n[2]*n[11] + n[4]*n[3]*n[10] + n[8]*n[2]*n[7]   - n[8]*n[3]*n[6];
        inv[11] = -n[0]*n[5]*n[11]  + n[0]*n[7]*n[9]   + n[4]*n[1]*n[11] - n[4]*n[3]*n[9]  - n[8]*n[1]*n[7]   + n[8]*n[3]*n[5];
        inv[15] =  n[0]*n[5]*n[10]  - n[0]*n[6]*n[9]   - n[4]*n[1]*n[10] + n[4]*n[2]*n[9]  + n[8]*n[1]*n[6]   - n[8]*n[2]*n[5];

        const T det = n[0]*inv[0] + n[1]*inv[4] + n[2]*inv[8] + n[3]*inv[12];
        if (!Debug::Assert(!AreValuesClose(det, static_cast<T>(0)), "Matrix is singular, cannot invert"))
        {
            return Matrix44();
        }

        const T invDet = static_cast<T>(1) / det;

        Matrix44 result;
        for (usize col = 0; col < 4; ++col)
        {
            for (usize row = 0; row < 4; ++row)
            {
                result.m[col][row] = inv[col * 4 + row] * invDet;
            }
        }
        return result;
    }

    using Mat44  = Matrix44<float32>;
    using Mat44d = Matrix44<float64>;
}
