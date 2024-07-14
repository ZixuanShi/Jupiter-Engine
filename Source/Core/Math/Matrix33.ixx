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

namespace jpt
{
	export template<Numeric T>
	struct Matrix33
	{
	public:
		Vector3<T> m[3];

	public:
		static consteval Matrix33<T> Identity() { return Matrix33<T>(); }

	public:
		constexpr Matrix33();
		constexpr Matrix33(const Vector3<T>& xAxis, const Vector3<T>& yAxis, const Vector3<T>& zAxis);
		constexpr Matrix33(T m00, T m01, T m02,
						   T m10, T m11, T m12,
						   T m20, T m21, T m22);

		constexpr Vector2<T> operator*(Vector2<T> v) const;
		constexpr Matrix33<T> operator*(const Matrix33<T>& rhs) const;

		constexpr T Determinant() const;

		constexpr static Matrix33 Translation(Vector2<T> v);
		constexpr static Matrix33 Rotation(T radians);
		constexpr static Matrix33 RotationDegrees(T degrees);
		constexpr static Matrix33 Scaling(Vector2<T> v);
		constexpr static Matrix33 Scaling(T scalar);

		constexpr void Translate(Vector2<T> v);
		constexpr void Rotate(T radians);
		constexpr void RotateDegrees(T degrees);
		constexpr void Scale(Vector2<T> v);
		constexpr void Scale(T scalar);

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
	constexpr void Matrix33<T>::Translate(Vector2<T> v)
	{
		*this = Translation(v) * *this; 
	}

	template<Numeric T>
	constexpr void Matrix33<T>::Rotate(T radians)
	{
		*this = Rotation(radians) * *this;
	}

	template<Numeric T>
	constexpr void Matrix33<T>::RotateDegrees(T degrees)
	{
		*this = RotationDegrees(degrees) * *this;
	}

	template<Numeric T>
	constexpr void Matrix33<T>::Scale(Vector2<T> v)
	{
		*this = Scaling(v) * *this;
	}

	template<Numeric T>
	constexpr void Matrix33<T>::Scale(T scalar)
	{
		*this = Scaling(scalar) * *this;
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
		jpt::String result = "\n";

		result += jpt::ToString(m[0][0]) + ", " + jpt::ToString(m[0][1]) + ", " + jpt::ToString(m[0][2]) + "\n";
		result += jpt::ToString(m[1][0]) + ", " + jpt::ToString(m[1][1]) + ", " + jpt::ToString(m[1][2]) + "\n";
		result += jpt::ToString(m[2][0]) + ", " + jpt::ToString(m[2][1]) + ", " + jpt::ToString(m[2][2]);

		return result;
	}
}

export using Matrix33f = jpt::Matrix33<float32>;