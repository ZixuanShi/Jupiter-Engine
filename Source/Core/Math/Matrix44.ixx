// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Matrix44;

import jpt.Concepts;
import jpt.Constants;
import jpt.TypeDefs;
import jpt.Math;
import jpt.Vector3;
import jpt.Vector4;
import jpt.Utilities;
import jpt.String;
import jpt.ToString;

namespace jpt
{
	export template<Numeric T>
	struct Matrix44
	{
	public:
		Vector4<T> m[4];

	public:
		using NumericType = T;

		static consteval Matrix44 Identity() { return Matrix44(); }

	public:
		constexpr Matrix44();
		constexpr Matrix44(const Vector4<T>& v0, const Vector4<T>& v1, const Vector4<T>& v2, const Vector4<T>& v3);
		constexpr Matrix44(T m00, T m01, T m02, T m03,
		                   T m10, T m11, T m12, T m13,
		                   T m20, T m21, T m22, T m23,
		                   T m30, T m31, T m32, T m33);

		constexpr Matrix44<T> operator*(const Matrix44<T>& rhs) const;
		constexpr Vector3<T> operator*(const Vector3<T>& rhs) const;
		constexpr Vector4<T> operator*(const Vector4<T>& rhs) const;
		constexpr Matrix44<T>& operator*=(const Matrix44<T>& rhs);

		constexpr T Determinant() const;

		constexpr static Matrix44<T> Translation(const Vector3<T>& v);
		constexpr static Matrix44<T> RotationX(T radians);
		constexpr static Matrix44<T> RotationY(T radians);
		constexpr static Matrix44<T> RotationZ(T radians);
		constexpr static Matrix44<T> Scaling(const Vector3<T>& v);
		constexpr static Matrix44<T> Transposed(const Matrix44<T>& matrix44);
		constexpr static Matrix44<T> Orthographic(T left, T right, T bottom, T top, T near, T far);
		constexpr static Matrix44<T> FromDegrees(const Vector3<T>& degrees);
		constexpr static Matrix44<T> FromDegrees(T pitch, T yaw, T row);

		constexpr void Translate(const Vector3<T>& v);
		constexpr void RotateX(T radians);
		constexpr void RotateY(T radians);
		constexpr void RotateZ(T radians);
		constexpr void Scale(const Vector3<T>& v);
		constexpr void Transpose();
		constexpr void Inverse();
		constexpr bool IsOrthogonal() const;

		constexpr String ToString() const;
	};

	template<Numeric T>
	constexpr Matrix44<T>::Matrix44()
		: m{ { 1, 0, 0, 0 }, 
		     { 0, 1, 0, 0 }, 
		     { 0, 0, 1, 0 }, 
		     { 0, 0, 0, 1 } }
	{
	}

	template<Numeric T>
	constexpr Matrix44<T>::Matrix44(const Vector4<T>& v0, const Vector4<T>& v1, const Vector4<T>& v2, const Vector4<T>& v3)
		: m{ v0, v1, v2, v3 }
	{
	}
	template<Numeric T>
	constexpr Matrix44<T>::Matrix44(T m00, T m01, T m02, T m03, 
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
	constexpr Matrix44<T> Matrix44<T>::operator*(const Matrix44<T>& rhs) const
	{
		Matrix44<T> result;

		for (size_t i = 0; i < 4; ++i)
		{
			for (size_t j = 0; j < 4; ++j)
			{
				result.m[i][j] = m[i][0] * rhs.m[0][j] + 
					             m[i][1] * rhs.m[1][j] + 
					             m[i][2] * rhs.m[2][j] + 
					             m[i][3] * rhs.m[3][j];
			}
		}

		return result;
	}

	template<Numeric T>
	constexpr Vector3<T> Matrix44<T>::operator*(const Vector3<T>& rhs) const
	{
		Vector4<T> result = *this * Vector4<T>(rhs, 1);
		return Vector3<T>(result.x, result.y, result.z);
	}

	template<Numeric T>
	constexpr Vector4<T> Matrix44<T>::operator*(const Vector4<T>& rhs) const
	{
		Vector4<T> result;
		for (size_t i = 0; i < 4; ++i)
		{
			result[i] = m[i][0] * rhs[0] + 
				        m[i][1] * rhs[1] + 
				        m[i][2] * rhs[2] + 
				        m[i][3] * rhs[3];
		}
		return result;
	}

	template<Numeric T>
	constexpr Matrix44<T>& Matrix44<T>::operator*=(const Matrix44<T>& rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	template<Numeric T>
	constexpr T Matrix44<T>::Determinant() const
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
	constexpr Matrix44<T> Matrix44<T>::Translation(const Vector3<T>& v)
	{
		return Matrix44<T>(1, 0, 0, v.x,
			               0, 1, 0, v.y,
			               0, 0, 1, v.z,
			               0, 0, 0,   1);
	}

	template<Numeric T>
	constexpr Matrix44<T> Matrix44<T>::RotationX(T radians)
	{
		const T cos = std::cos(radians);
		const T sin = std::sin(radians);
		return Matrix44<T>(1,   0,    0, 0,
			               0, cos, -sin, 0,
			               0, sin,  cos, 0,
			               0,   0,    0, 1);
	}

	template<Numeric T>
	constexpr Matrix44<T> Matrix44<T>::RotationY(T radians)
	{
		const T cos = std::cos(radians);
		const T sin = std::sin(radians);
		return Matrix44<T>( cos, 0, sin, 0,
			                  0, 1,   0, 0,
			               -sin, 0, cos, 0,
			                  0, 0,   0, 1);
	}

	template<Numeric T>
	constexpr Matrix44<T> Matrix44<T>::RotationZ(T radians)
	{
		const T cos = std::cos(radians);
		const T sin = std::sin(radians);
		return Matrix44<T>(cos, -sin, 0, 0,
			               sin,  cos, 0, 0,
			                 0,    0, 1, 0,
			                 0,    0, 0, 1);
	}

	template<Numeric T>
	constexpr Matrix44<T> Matrix44<T>::Scaling(const Vector3<T>& v)
	{
		return Matrix44<T>(v.x,   0,   0, 0,
			                 0, v.y,   0, 0,
			                 0,   0, v.z, 0,
			                 0,   0,   0, 1);
	}

	template<Numeric T>
	constexpr Matrix44<T> Matrix44<T>::Transposed(const Matrix44<T>& matrix44)
	{
		Matrix44<T> result = matrix44;
		result.Transpose();
		return result;
	}

	template<Numeric T>
	constexpr Matrix44<T> Matrix44<T>::Orthographic(T left, T right, T bottom, T top, T near, T far)
	{
		Matrix44<T> result = Matrix44<T>::Identity();

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
	constexpr Matrix44<T> Matrix44<T>::FromDegrees(const Vector3<T>& degrees)
	{
		// Step 1: Convert degrees to radians
		const Vector3<T> radians = ToRadians(degrees);

		// Step 2: Calculate the cosine and sine of the angles
		const T cx = std::cos(radians.x);
		const T cy = std::cos(radians.y);
		const T cz = std::cos(radians.z);
		const T sx = std::sin(radians.x);
		const T sy = std::sin(radians.y);
		const T sz = std::sin(radians.z);

		// Step 3: Apply the rotation matrix formula
		const T m00 = cy * cz;
		const T m01 = -cy * sz;
		const T m02 = sy;
		const T m10 = cx * sz + sx * sy * cz;
		const T m11 = cx * cz - sx * sy * sz;
		const T m12 = -sx * cy;
		const T m20 = sx * sz - cx * sy * cz;
		const T m21 = sx * cz + cx * sy * sz;
		const T m22 = cx * cy;

		// Step 4: Fill in the matrix
		return Matrix44<T>(m00, m01, m02, 0,
			               m10, m11, m12, 0,
			               m20, m21, m22, 0,
			               0,   0,   0,   1);
	}

	template<Numeric T>
	constexpr Matrix44<T> Matrix44<T>::FromDegrees(T pitch, T yaw, T row)
	{
		return FromDegrees(Vector3<T>(pitch, yaw, row));
	}

	template<Numeric T>
	constexpr void Matrix44<T>::Translate(const Vector3<T>& v)
	{
		*this *= Translation(v);
	}

	template<Numeric T>
	constexpr void Matrix44<T>::RotateX(T radians)
	{
		*this *= RotationX(radians);
	}

	template<Numeric T>
	constexpr void Matrix44<T>::RotateY(T radians)
	{
		*this *= RotationY(radians);
	}

	template<Numeric T>
	constexpr void Matrix44<T>::RotateZ(T radians)
	{
		*this *= RotationZ(radians);
	}

	template<Numeric T>
	constexpr void Matrix44<T>::Scale(const Vector3<T>& v)
	{
		*this *= Scaling(v);
	}

	template<Numeric T>
	constexpr void Matrix44<T>::Transpose()
	{
		Swap(m[0][1], m[1][0]);
		Swap(m[0][2], m[2][0]);
		Swap(m[0][3], m[3][0]);
		Swap(m[1][2], m[2][1]);
		Swap(m[1][3], m[3][1]);
		Swap(m[2][3], m[3][2]);
	}

	template<Numeric T>
	constexpr void Matrix44<T>::Inverse()
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
	constexpr bool Matrix44<T>::IsOrthogonal() const
	{
		const T dotX = m[0].Dot(m[1]);
		const T dotY = m[1].Dot(m[2]);
		const T dotZ = m[2].Dot(m[0]);

		return AreValuesClose(dotX, static_cast<T>(0)) &&
			   AreValuesClose(dotY, static_cast<T>(0)) &&
			   AreValuesClose(dotZ, static_cast<T>(0));
	}

	template<Numeric T>
	constexpr String Matrix44<T>::ToString() const
	{
		return String::Format<256>("\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f", m[0][0], m[0][1], m[0][2], m[0][3],
			                                                                                                                           m[1][0], m[1][1], m[1][2], m[1][3],
			                                                                                                                           m[2][0], m[2][1], m[2][2], m[2][3],
			                                                                                                                           m[3][0], m[3][1], m[3][2], m[3][3]);
	}
}

export using Matrix44f = jpt::Matrix44<float32>;