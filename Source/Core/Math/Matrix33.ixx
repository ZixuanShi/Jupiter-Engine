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

namespace jpt
{
	export template<Numeric T>
	struct Matrix33
	{
	public:
		Vector3<T> m[3];

	public:
		static constexpr Matrix33 Identity() { return Matrix33<T>(); }

	public:
		constexpr Matrix33();
		constexpr Matrix33(const Vector3<T>& xAxis, const Vector3<T>& yAxis, const Vector3<T>& zAxis);
		constexpr Matrix33(T m00, T m01, T m02,
						   T m10, T m11, T m12,
						   T m20, T m21, T m22);

		constexpr Vector2<T> operator*(Vector2<T> v) const;
		constexpr Matrix33<T> operator*(const Matrix33<T>& rhs) const;

		constexpr void Translate(Vector2<T> v);
		constexpr void Rotate(T radians);
		constexpr void RotateDegrees(T degrees);
		constexpr void Scale(Vector2<T> v);

		constexpr static Matrix33<T> Translate(const Matrix33<T>& matrix, Vector2<T> v);
		constexpr static Matrix33<T> Rotate(const Matrix33<T>& matrix, T radians);
		constexpr static Matrix33<T> RotateDegrees(const Matrix33<T>& matrix, T degrees);
		constexpr static Matrix33<T> Scale(const Matrix33<T>& matrix, Vector2<T> v);

		constexpr bool operator==(const Matrix33<T>& rhs) const;
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
		T x = m[0][0] * v.x + m[0][1] * v.y + m[0][2];
		T y = m[1][0] * v.x + m[1][1] * v.y + m[1][2];
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
				result.m[i][j] = m[i][0] * rhs.m[0][j] + m[i][1] * rhs.m[1][j] + m[i][2] * rhs.m[2][j];
			}
		}

		return result;
	}

	template<Numeric T>
	constexpr void Matrix33<T>::Translate(Vector2<T> v)
	{
		m[0][2] += v.x;
		m[1][2] += v.y;
	}

	template<Numeric T>
	constexpr void Matrix33<T>::Rotate(T radians)
	{
		const T cos = std::cos(radians);
		const T sin = std::sin(radians);

		Matrix33 rotationMatrix;

		rotationMatrix.m[0][0] =  cos;
		rotationMatrix.m[0][1] = -sin;
		rotationMatrix.m[1][0] =  sin;
		rotationMatrix.m[1][1] =  cos;

		*this = rotationMatrix * (*this);
	}

	template<Numeric T>
	constexpr void Matrix33<T>::RotateDegrees(T degrees)
	{
		Rotate(DegreesToRadians(degrees));
	}

	template<Numeric T>
	constexpr void Matrix33<T>::Scale(Vector2<T> v)
	{
		m[0][0] *= v.x;
		m[1][1] *= v.y;
	}

	template<Numeric T>
	constexpr Matrix33<T> Matrix33<T>::Translate(const Matrix33<T>& matrix, Vector2<T> v)
	{
		Matrix33<T> result = matrix;
		result.Translate(v);
		return result;
	}

	template<Numeric T>
	constexpr Matrix33<T> Matrix33<T>::Rotate(const Matrix33<T>& matrix, T radians)
	{
		Matrix33<T> result = matrix;
		result.Rotate(radians);
		return result;
	}

	template<Numeric T>
	constexpr Matrix33<T> Matrix33<T>::RotateDegrees(const Matrix33<T>& matrix, T degrees)
	{
		Matrix33<T> result = matrix;
		result.RotateDegrees(degrees);
		return result;
	}

	template<Numeric T>
	constexpr Matrix33<T> Matrix33<T>::Scale(const Matrix33<T>& matrix, Vector2<T> v)
	{
		Matrix33<T> result = matrix;
		result.Scale(v);
		return result;
	}

	template<Numeric T>
	constexpr bool Matrix33<T>::operator==(const Matrix33<T>& rhs) const
	{
		for (size_t i = 0; i < 3; ++i)
		{
			for (size_t j = 0; j < 3; ++j)
			{
				if (!AreValuesClose(m[i][j], rhs.m[i][j]))
				{
					return false;
				}
			}
		}

		return true;
	}
}

export using Matrix33f = jpt::Matrix33<float32>;