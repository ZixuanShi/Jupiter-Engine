// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <cmath>

export module jpt.Matrix44;

import jpt.Concepts;
import jpt.Constants;
import jpt.TypeDefs;
import jpt.Math;
import jpt.Math.Settings;
import jpt.Vector3;
import jpt.Vector4;
import jpt.Utilities;
import jpt.String;
import jpt.ToString;

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
		constexpr TMatrix44<T>  operator* (const TMatrix44<T>& rhs) const;
		constexpr TMatrix44<T>& operator*=(const TMatrix44<T>& rhs);
		constexpr Vector3<T>    operator* (const Vector3<T>&  rhs) const;
		constexpr Vector4<T>    operator* (const Vector4<T>&   rhs) const;

		constexpr       Vector4<T>& operator[](size_t index)       { return m[index]; }
		constexpr const Vector4<T>& operator[](size_t index) const { return m[index]; }

	public:
		// Translation & Position
		constexpr static TMatrix44<T> Translation(const Vector3<T>& v);
		constexpr static TMatrix44<T> Translation(T x, T y, T z);
		constexpr void Translate(const Vector3<T>& v);
		constexpr void Translate(T x, T y, T z);
		constexpr Vector3<T> GetPosition() const;

		// Rotation & Orientation
		constexpr static TMatrix44<T> RotationX(T radians);
		constexpr static TMatrix44<T> RotationY(T radians);
		constexpr static TMatrix44<T> RotationZ(T radians);
		constexpr static TMatrix44<T> Rotation(const Vector3<T>& radians);
		constexpr static TMatrix44<T> Rotation(T pitch, T yaw, T roll);
		constexpr void RotateX(T radians);
		constexpr void RotateY(T radians);
		constexpr void RotateZ(T radians);
		constexpr void Rotate(const Vector3<T>& radians);
		constexpr void Rotate(T pitch, T yaw, T roll);
		constexpr Vector3<T> GetRotation() const;	// Euler Angles in Radians

		// Scaling & Size
		constexpr static TMatrix44<T> Scaling(const Vector3<T>& v);
		constexpr static TMatrix44<T> Scaling(T x, T y, T z);
		constexpr void Scale(const Vector3<T>& v);
		constexpr void Scale(T x, T y, T z);
		constexpr Vector3<T> GetScale() const;

		/** Measures the volume of the parallelepiped spanned by the vectors of the matrix.	If determinant is 0, matrix is not invertible. */
		constexpr T Determinant() const;

		/** Transpose. Swaps elements across the main diagonal. Used in checking orthogonality and normalizing the matrix. */
		constexpr static TMatrix44<T> Transposed(const TMatrix44<T>& matrix);
		constexpr void Transpose();

		/** Inverse matrix's behaviors. Undo */
		constexpr static TMatrix44<T> Inverse(const TMatrix44<T>& matrix);
		constexpr void Invert();

		/** @return true if matrix is orthogonal. Validates that a matrix only contains rotation (no scaling/shearing) */
		constexpr bool IsOrthogonal() const;

		/** Creates an orthographic projection matrix */
		constexpr static TMatrix44<T> Orthographic(T left, T right, T bottom, T top, T near, T far);

		/** Creates a view matrix for a camera, defining how the world is oriented relative to the camera's position */
		constexpr static TMatrix44<T> LookAt(const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up = Vector3<T>::Up());

		/** Converts 3D coordinates into 2D screen coordinates */
		constexpr static TMatrix44<T> Perspective(T fov, T aspect, T near, T far);

		// Utils
		constexpr String ToString() const;
	};

	template<Numeric T>
	constexpr bool operator==(const TMatrix44<T>& lhs, const TMatrix44<T>& rhs)
	{
		return lhs.m[0] == rhs.m[0] &&
			   lhs.m[1] == rhs.m[1] &&
			   lhs.m[2] == rhs.m[2] &&
			   lhs.m[3] == rhs.m[3];
	}

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
	constexpr TMatrix44<T> TMatrix44<T>::operator*(const TMatrix44<T>& rhs) const
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
	constexpr Vector3<T> TMatrix44<T>::operator*(const Vector3<T>& rhs) const
	{
		Vector4<T> result = *this * Vector4<T>(rhs, 1);
		return Vector3<T>(result.x, result.y, result.z);
	}

	template<Numeric T>
	constexpr Vector4<T> TMatrix44<T>::operator*(const Vector4<T>& rhs) const
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
	constexpr TMatrix44<T>& TMatrix44<T>::operator*=(const TMatrix44<T>& rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Translation(const Vector3<T>& v)
	{
		return TMatrix44<T>(  1,   0,   0, 0,
			                  0,   1,   0, 0,
			                  0,   0,   1, 0,
			                v.x, v.y, v.z, 1);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Translation(T x, T y, T z)
	{
		return TMatrix44<T>(1, 0, 0, 0,
						    0, 1, 0, 0,
						    0, 0, 1, 0,
						    x, y, z, 1);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::Translate(const Vector3<T>& v)
	{
		*this *= Translation(v);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::Translate(T x, T y, T z)
	{
		*this *= Translation(x, y, z);
	}

	template<Numeric T>
	constexpr Vector3<T> TMatrix44<T>::GetPosition() const
	{
		return Vector3<T>(m[3].x, m[3].y, m[3].z);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::RotationX(T radians)
	{
		const T cos = Cos(radians);
		const T sin = Sin(radians);
		return TMatrix44<T>(1,   0,    0,  0,
			                0,  cos,  sin, 0,
			                0, -sin,  cos, 0,
			                0,    0,    0, 1);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::RotationY(T radians)
	{
		const T cos = Cos(radians);
		const T sin = Sin(radians);
		return TMatrix44<T>( cos, 0, -sin, 0,
			                   0, 1,    0, 0,
			                 sin, 0,  cos, 0,
			                   0, 0,    0, 1);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::RotationZ(T radians)
	{
		const T cos = Cos(radians);
		const T sin = Sin(radians);
		return TMatrix44<T>( cos,  sin, 0, 0,
			                -sin,  cos, 0, 0,
			                   0,    0, 1, 0,
			                   0,    0, 0, 1);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Rotation(const Vector3<T>& radians)
	{
		return RotationX(radians.x) * RotationY(radians.y) * RotationZ(radians.z);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Rotation(T pitch, T yaw, T roll)
	{
		return RotationX(pitch) * RotationY(yaw) * RotationZ(roll);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::RotateX(T radians)
	{
		*this *= RotationX(radians);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::RotateY(T radians)
	{
		*this *= RotationY(radians);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::RotateZ(T radians)
	{
		*this *= RotationZ(radians);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::Rotate(const Vector3<T>& radians)
	{
		*this *= Rotation(radians);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::Rotate(T pitch, T yaw, T roll)
	{
		*this *= Rotation(pitch, yaw, roll);
	}

	template<Numeric T>
	constexpr Vector3<T> TMatrix44<T>::GetRotation() const
	{
		// Extracting Euler Angles from a Rotation Matrix

		const Vector3<T> scale = GetScale();
		TMatrix44<T> rotation = *this;
		rotation[0] = Vector4<T>(m[0][0] / scale.x, m[0][1] / scale.x, m[0][2] / scale.x, 0);
		rotation[1] = Vector4<T>(m[1][0] / scale.y, m[1][1] / scale.y, m[1][2] / scale.y, 0);
		rotation[2] = Vector4<T>(m[2][0] / scale.z, m[2][1] / scale.z, m[2][2] / scale.z, 0);
		rotation[3] = Vector4<T>(0, 0, 0, 1);

		const T sy = Sqrt(rotation[0][0] * rotation[0][0] + rotation[1][0] * rotation[1][0]);
		if (sy > Constants<T>::kEpsilon)
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
	constexpr TMatrix44<T> TMatrix44<T>::Scaling(const Vector3<T>& v)
	{
		return TMatrix44<T>(v.x,  0,   0, 0,
			                 0, v.y,   0, 0,
			                 0,   0, v.z, 0,
			                 0,   0,   0, 1);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Scaling(T x, T y, T z)
	{
		return TMatrix44<T>(x, 0, 0, 0,
							0, y, 0, 0,
							0, 0, z, 0,
							0, 0, 0, 1);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::Scale(const Vector3<T>& v)
	{
		*this *= Scaling(v);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::Scale(T x, T y, T z)
	{
		*this *= Scaling(x, y, z);
	}

	template<Numeric T>
	constexpr Vector3<T> TMatrix44<T>::GetScale() const
	{
		const T x = Vector3<T>(m[0].x, m[0].y, m[0].z).Length();
		const T y = Vector3<T>(m[1].x, m[1].y, m[1].z).Length();
		const T z = Vector3<T>(m[2].x, m[2].y, m[2].z).Length();
		return Vector3<T>(x, y, z);
	}

	template<Numeric T>
	constexpr T TMatrix44<T>::Determinant() const
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
		const T m = m[3][0];
		const T n = m[3][1];
		const T o = m[3][2];
		const T p = m[3][3];

		const T det = a * f * k * p + a * g * l * n + a * h * j * o +
			          b * e * l * p + b * g * i * p + b * h * k * n +
			          c * e * j * p + c * f * l * m + c * h * i * m +
			          d * e * k * n + d * f * i * o + d * g * j * m -
			          a * f * l * o - a * g * j * p - a * h * k * m -
			          b * e * k * o - b * g * l * m - b * h * i * p -
			          c * e * l * n - c * f * i * p - c * h * j * m -
			          d * e * j * n - d * f * k * m - d * g * i * l;

		return det;
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Transposed(const TMatrix44<T>& matrix)
	{
		return TMatrix44<T>(matrix.m[0][0], matrix.m[1][0], matrix.m[2][0], matrix.m[3][0],
							matrix.m[0][1], matrix.m[1][1], matrix.m[2][1], matrix.m[3][1],
							matrix.m[0][2], matrix.m[1][2], matrix.m[2][2], matrix.m[3][2],
							matrix.m[0][3], matrix.m[1][3], matrix.m[2][3], matrix.m[3][3]);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::Transpose()
	{
		Swap(m[0][1], m[1][0]);
		Swap(m[0][2], m[2][0]);
		Swap(m[0][3], m[3][0]);
		Swap(m[1][2], m[2][1]);
		Swap(m[1][3], m[3][1]);
		Swap(m[2][3], m[3][2]);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Inverse(const TMatrix44<T>& matrix)
	{
		TMatrix44<T> result = matrix;
		result.Invert();
		return result;
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::Invert()
	{
		const T det = Determinant();
		if (det == 0)
		{
			return;
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
		const T m = m[3][0];
		const T n = m[3][1];
		const T o = m[3][2];
		const T p = m[3][3];

		m[0][0] = (f * k * p + g * l * n + h * j * o - f * l * o - g * j * p - h * k * n) * invDet;
		m[0][1] = (b * l * o + c * j * p + d * k * n - b * k * p - c * l * n - d * j * o) * invDet;
		m[0][2] = (b * g * p + c * h * n + d * f * o - b * h * o - c * f * p - d * g * n) * invDet;
		m[0][3] = (b * h * k + c * f * l + d * g * j - b * g * l - c * h * j - d * f * k) * invDet;
		m[1][0] = (e * l * o + g * i * p + h * k * m - e * k * p - g * l * m - h * i * o) * invDet;
		m[1][1] = (a * k * p + c * l * m + d * i * o - a * l * o - c * i * p - d * k * m) * invDet;
		m[1][2] = (a * h * o + c * f * p + d * g * m - a * g * p - c * h * m - d * f * o) * invDet;
		m[1][3] = (a * g * l + c * h * m + d * f * k - a * h * k - c * f * l - d * g * m) * invDet;
		m[2][0] = (e * j * p + f * l * m + h * i * n - e * l * n - f * i * p - h * j * m) * invDet;
		m[2][1] = (a * l * n + b * i * p + d * j * m - a * j * p - b * l * m - d * i * n) * invDet;
		m[2][2] = (a * f * p + b * h * m + d * e * n - a * h * n - b * e * p - d * f * m) * invDet;
		m[2][3] = (a * h * j + b * e * l + d * f * i - a * f * l - b * h * i - d * e * j) * invDet;
		m[3][0] = (e * k * n + f * i * o + g * j * m - e * j * o - f * k * m - g * i * n) * invDet;
		m[3][1] = (a * j * o + b * k * m + c * i * n - a * k * n - b * i * o - c * j * m) * invDet;
		m[3][2] = (a * g * n + b * e * o + c * f * m - a * f * o - b * g * m - c * e * n) * invDet;
		m[3][3] = (a * f * k + b * g * m + c * e * j - a * g * j - b * e * k - c * f * m) * invDet;
	}

	template<Numeric T>
	constexpr bool TMatrix44<T>::IsOrthogonal() const
	{
		const TMatrix44<T> inverse = Inverse(*this);
		const TMatrix44<T> transposed = Transposed(*this);
		return inverse == transposed;
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Orthographic(T left, T right, T bottom, T top, T near, T far)
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
	constexpr TMatrix44<T> TMatrix44<T>::LookAt(const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up /* = Vector3<T>::Up()*/)
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
	constexpr TMatrix44<T> TMatrix44<T>::Perspective(T fov, T aspect, T zNear, T zFar)
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
	constexpr String TMatrix44<T>::ToString() const
	{
		return String::Format<256>("\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f", m[0][0], m[0][1], m[0][2], m[0][3],
			                                                                                                                           m[1][0], m[1][1], m[1][2], m[1][3],
			                                                                                                                           m[2][0], m[2][1], m[2][2], m[2][3],
			                                                                                                                           m[3][0], m[3][1], m[3][2], m[3][3]);
	}
}

export using Matrix44  = jpt::TMatrix44<Precision>;
export using Matrix44f = jpt::TMatrix44<float32>;