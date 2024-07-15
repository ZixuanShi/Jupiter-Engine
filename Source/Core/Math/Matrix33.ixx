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
import jpt.ToString;
import jpt.Utilities;

namespace jpt
{
	export template<Numeric T>
	struct Matrix33
	{
	public:
		Vector3<T> m[3];

	public:
		static consteval Matrix33 Identity() { return Matrix33(); }

	public:
		constexpr Matrix33();
		constexpr Matrix33(const Vector3<T>& xAxis, const Vector3<T>& yAxis, const Vector3<T>& zAxis);
		constexpr Matrix33(T m00, T m01, T m02,
						   T m10, T m11, T m12,
						   T m20, T m21, T m22);

		constexpr Vector2<T> operator*(Vector2<T> v) const;
		constexpr Matrix33<T> operator*(const Matrix33<T>& rhs) const;
		constexpr Matrix33<T>& operator*=(const Matrix33<T>& rhs);

		constexpr T Determinant() const;

		constexpr static Matrix33 Translation(Vector2<T> v);
		constexpr static Matrix33 Rotation(T radians);
		constexpr static Matrix33 RotationDegrees(T degrees);
		constexpr static Matrix33 Scaling(Vector2<T> v);
		constexpr static Matrix33 Scaling(T scalar);
		constexpr static Matrix33 Orthographic(T left, T right, T bottom, T top, T near, T far);

		constexpr void Translate(Vector2<T> v);
		constexpr void Rotate(T radians);
		constexpr void RotateDegrees(T degrees);
		constexpr void Scale(Vector2<T> v);
		constexpr void Scale(T scalar);
		constexpr void Transpose();
		constexpr void Inverse();
		constexpr bool IsOrthogonal() const;

		constexpr bool operator==(const Matrix33<T>& rhs) const;

		constexpr String ToString() const;
	};

	template<Numeric T>
	constexpr Matrix33<T>::Matrix33()
		: m{ { 1, 0, 0 },
			 { 0, 1, 0 },
			 { 0, 0, 1 } }
	{
	}

	template<Numeric T>
	constexpr Matrix33<T>::Matrix33(const Vector3<T>& xAxis, const Vector3<T>& yAxis, const Vector3<T>& zAxis)
		: m{ xAxis, yAxis, zAxis }
	{
	}

	template<Numeric T>
	constexpr Matrix33<T>::Matrix33(T m00, T m01, T m02, 
		                            T m10, T m11, T m12, 
		                            T m20, T m21, T m22)
		: m{ { m00, m01, m02 },
			 { m10, m11, m12 },
			 { m20, m21, m22 } }
	{
	}

	template<Numeric T>
	constexpr Vector2<T> Matrix33<T>::operator*(Vector2<T> v) const
	{
		const T x = m[0][0] * v.x + m[0][1] * v.y + m[0][2];
		const T y = m[1][0] * v.x + m[1][1] * v.y + m[1][2];
		return Vector2<T>(x, y);
	}

	template<Numeric T>
	constexpr Matrix33<T> Matrix33<T>::operator*(const Matrix33<T>& rhs) const
	{
		Matrix33<T> result;

		for (size_t i = 0; i < 3; ++i)
		{
			for (size_t j = 0; j < 3; ++j)
			{
				result.m[i][j] = m[i][0] * rhs.m[0][j] +
								 m[i][1] * rhs.m[1][j] +
								 m[i][2] * rhs.m[2][j];
			}
		}

		return result;
	}

	template<Numeric T>
	constexpr Matrix33<T>& Matrix33<T>::operator*=(const Matrix33<T>& rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	template<Numeric T>
	constexpr T Matrix33<T>::Determinant() const
	{
		return m[0][0] * m[1][1] * m[2][2] +
			   m[0][1] * m[1][2] * m[2][0] +
			   m[0][2] * m[1][0] * m[2][1] -
			   m[0][2] * m[1][1] * m[2][0] -
			   m[0][1] * m[1][0] * m[2][2] -
			   m[0][0] * m[1][2] * m[2][1];
	}

	template<Numeric T>
	constexpr Matrix33<T> Matrix33<T>::Translation(Vector2<T> v)
	{
		return Matrix33<T>(1, 0, v.x,
					       0, 1, v.y,
			               0, 0,   1);
	}

	template<Numeric T>
	constexpr Matrix33<T> Matrix33<T>::Rotation(T radians)
	{
		const T cos = std::cos(radians);
		const T sin = std::sin(radians);

		return Matrix33<T>(cos, -sin, 0,
			               sin,  cos, 0,
			                 0,    0, 1);
	}

	template<Numeric T>
	constexpr Matrix33<T> Matrix33<T>::RotationDegrees(T degrees)
	{
		return Rotation(ToRadians(degrees));
	}

	template<Numeric T>
	constexpr Matrix33<T> Matrix33<T>::Scaling(Vector2<T> v)
	{
		return Matrix33<T>(v.x,   0, 0,
			                 0, v.y, 0,
			                 0,   0, 1);
	}

	template<Numeric T>
	constexpr Matrix33<T> Matrix33<T>::Scaling(T scalar)
	{
		return Matrix33<T>(scalar,      0, 0,
			                    0, scalar, 0,
			                    0,      0, 1);
	}

	template<Numeric T>
	constexpr Matrix33<T> Matrix33<T>::Orthographic(T left, T right, T bottom, T top, T near, T far)
	{
		Matrix33<T> result = Identity();

		const T width = right - left;
		const T height = top - bottom;
		const T depth = far - near;

		result.m[0][0] = 2 / width;
		result.m[1][1] = 2 / height;
		result.m[2][2] = -2 / depth;
		result.m[0][2] = -(right + left) / width;
		result.m[1][2] = -(top + bottom) / height;
		result.m[2][2] = -(far + near) / depth;

		return result;
	}

	template<Numeric T>
	constexpr void Matrix33<T>::Translate(Vector2<T> v)
	{
		*this *= Translation(v); 
	}

	template<Numeric T>
	constexpr void Matrix33<T>::Rotate(T radians)
	{
		*this *= Rotation(radians);
	}

	template<Numeric T>
	constexpr void Matrix33<T>::RotateDegrees(T degrees)
	{
		*this *= RotationDegrees(degrees);
	}

	template<Numeric T>
	constexpr void Matrix33<T>::Scale(Vector2<T> v)
	{
		*this *= Scaling(v);
	}

	template<Numeric T>
	constexpr void Matrix33<T>::Scale(T scalar)
	{
		*this *= Scaling(scalar);
	}

	template<Numeric T>
	constexpr void Matrix33<T>::Transpose()
	{
		Swap(m[0][1], m[1][0]);
		Swap(m[0][2], m[2][0]);
		Swap(m[1][2], m[2][1]);
	}

	template<Numeric T>
	constexpr void Matrix33<T>::Inverse()
	{
		const T det = Determinant();
		if (det == 0)
		{
			return;
		}

		const T invDet = 1 / det;

		Matrix33<T> result;
		result.m[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
		result.m[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet;
		result.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;
		result.m[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet;
		result.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
		result.m[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * invDet;
		result.m[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
		result.m[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * invDet;
		result.m[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;

		*this = result;
	}

	template<Numeric T>
	constexpr bool Matrix33<T>::IsOrthogonal() const
	{
		const T dotX = m[0].Dot(m[1]);
		const T dotY = m[1].Dot(m[2]);
		const T dotZ = m[2].Dot(m[0]);

		return AreValuesClose(dotX, static_cast<T>(0)) &&
			   AreValuesClose(dotY, static_cast<T>(0)) &&
			   AreValuesClose(dotZ, static_cast<T>(0));
	}

	template<Numeric T>
	constexpr bool Matrix33<T>::operator==(const Matrix33<T>& rhs) const
	{
		return m[0] == rhs.m[0] && 
			   m[1] == rhs.m[1] && 
			   m[2] == rhs.m[2];
	}

	template<Numeric T>
	constexpr String Matrix33<T>::ToString() const
	{
		return String::Format<128>("\n%.3f, %.3f, %.3f\n%.3f, %.3f, %.3f\n%.3f, %.3f, %.3f", m[0][0], m[0][1], m[0][2],
			                                                                                 m[1][0], m[1][1], m[1][2],
			                                                                                 m[2][0], m[2][1], m[2][2]);
	}
}

export using Matrix33f = jpt::Matrix33<float32>;