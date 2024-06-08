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

namespace jpt
{
	export template<Numeric T>
	struct Matrix44
	{
	public:
		Vector4<T> m[4];

	public:
		static constexpr Matrix44<T> Identity() { return Matrix44<T>(); }

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

		constexpr Matrix44<T> Transpose() const;

		constexpr static Matrix44<T> Translation(const Vector3<T>& v);
		constexpr static Matrix44<T> RotationX(T radians);
		constexpr static Matrix44<T> RotationY(T radians);
		constexpr static Matrix44<T> RotationZ(T radians);
		constexpr static Matrix44<T> Rotation(const Vector3<T>& axis, T radians);
		constexpr static Matrix44<T> Scaling(const Vector3<T>& v);
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
				result.m[i][j] = m[i][0] * rhs.m[0][j] + m[i][1] * rhs.m[1][j] + m[i][2] * rhs.m[2][j] + m[i][3] * rhs.m[3][j];
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
			result[i] = m[i][0] * rhs[0] + m[i][1] * rhs[1] + m[i][2] * rhs[2] + m[i][3] * rhs[3];
		}
		return result;
	}

	template<Numeric T>
	constexpr Matrix44<T> Matrix44<T>::Transpose() const
	{
		return Matrix44<T>(m[0][0], m[1][0], m[2][0], m[3][0],
			               m[0][1], m[1][1], m[2][1], m[3][1],
			               m[0][2], m[1][2], m[2][2], m[3][2],
			               m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	template<Numeric T>
	constexpr Matrix44<T> Matrix44<T>::Translation(const Vector3<T>& v)
	{
		return Matrix44<T>(1, 0, 0, v.x,
			               0, 1, 0, v.y,
			               0, 0, 1, v.z,
			               0, 0, 0, 1);
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
	constexpr Matrix44<T> Matrix44<T>::Rotation(const Vector3<T>& axis, T radians)
	{
		const T cos = std::cos(radians);
		const T sin = std::sin(radians);
		const T oneMinusCos = 1 - cos;
		const T x = axis.x;
		const T y = axis.y;
		const T z = axis.z;
		const T xy = x * y;
		const T xz = x * z;
		const T yz = y * z;
		const T xsin = x * sin;
		const T ysin = y * sin;
		const T zsin = z * sin;
		return Matrix44<T>(cos + x * x * oneMinusCos, xy * oneMinusCos - zsin, xz * oneMinusCos + ysin, 0,
			               xy * oneMinusCos + zsin, cos + y * y * oneMinusCos, yz * oneMinusCos - xsin, 0,
			               xz * oneMinusCos - ysin, yz * oneMinusCos + xsin, cos + z * z * oneMinusCos, 0,
			               0, 0, 0, 1);
	}

	template<Numeric T>
	constexpr Matrix44<T> Matrix44<T>::Scaling(const Vector3<T>& v)
	{
		return Matrix44<T>(v.x, 0, 0, 0,
			               0, v.y, 0, 0,
			               0, 0, v.z, 0,
			               0, 0, 0, 1);
	}
}

export using Matrix44f = jpt::Matrix44<float32>;