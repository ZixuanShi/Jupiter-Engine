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

export namespace jpt
{
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
		constexpr TMatrix33<T>  operator* (const TMatrix33<T>& rhs) const;
		constexpr TMatrix33<T>& operator*=(const TMatrix33<T>& rhs);
		constexpr Vector2<T>   operator* (Vector2<T> v) const;

	public:
		// Translation & Position
		constexpr static TMatrix33 Translation(Vector2<T> v);
		constexpr void Translate(Vector2<T> v);

		// Rotation & Orientation
		constexpr static TMatrix33 Rotation(T radians);
		constexpr void Rotate(T radians);

		// Scaling & Size
		constexpr static TMatrix33 Scaling(Vector2<T> v);
		constexpr static TMatrix33 Scaling(T scalar);
		constexpr void Scale(Vector2<T> v);
		constexpr void Scale(T scalar);

		// Utils
		constexpr String ToString() const;
	};

	template<Numeric T>
	constexpr bool operator==(const TMatrix33<T>& lhs, const TMatrix33<T>& rhs)
	{
		return lhs.m[0] == rhs.m[0] &&
			   lhs.m[1] == rhs.m[1] &&
			   lhs.m[2] == rhs.m[2];
	}

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
	constexpr TMatrix33<T> TMatrix33<T>::operator*(const TMatrix33<T>& rhs) const
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
	constexpr TMatrix33<T>& TMatrix33<T>::operator*=(const TMatrix33<T>& rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	template<Numeric T>
	constexpr Vector2<T> TMatrix33<T>::operator*(Vector2<T> v) const
	{
		const T x = m[0][0] * v.x + m[0][1] * v.y + m[0][2];
		const T y = m[1][0] * v.x + m[1][1] * v.y + m[1][2];
		return Vector2<T>(x, y);
	}

	template<Numeric T>
	constexpr TMatrix33<T> TMatrix33<T>::Translation(Vector2<T> v)
	{
		return TMatrix33<T>(1, 0, v.x,
					        0, 1, v.y,
			                0, 0,   1);
	}

	template<Numeric T>
	constexpr TMatrix33<T> TMatrix33<T>::Rotation(T radians)
	{
		const T cos = Cos(radians);
		const T sin = Sin(radians);

		return TMatrix33<T>(cos, -sin, 0,
			                sin,  cos, 0,
			                 0,    0, 1);
	}

	template<Numeric T>
	constexpr TMatrix33<T> TMatrix33<T>::Scaling(Vector2<T> v)
	{
		return TMatrix33<T>(v.x,   0, 0,
			                  0, v.y, 0,
			                  0,   0, 1);
	}

	template<Numeric T>
	constexpr TMatrix33<T> TMatrix33<T>::Scaling(T scalar)
	{
		return TMatrix33<T>(scalar,      0, 0,
			                     0, scalar, 0,
			                     0,      0, 1);
	}

	template<Numeric T>
	constexpr void TMatrix33<T>::Translate(Vector2<T> v)
	{
		*this *= Translation(v); 
	}

	template<Numeric T>
	constexpr void TMatrix33<T>::Rotate(T radians)
	{
		*this *= Rotation(radians);
	}

	template<Numeric T>
	constexpr void TMatrix33<T>::Scale(Vector2<T> v)
	{
		*this *= Scaling(v);
	}

	template<Numeric T>
	constexpr void TMatrix33<T>::Scale(T scalar)
	{
		*this *= Scaling(scalar);
	}

	template<Numeric T>
	constexpr String TMatrix33<T>::ToString() const
	{
		return String::Format<128>("\n%.3f, %.3f, %.3f\n%.3f, %.3f, %.3f\n%.3f, %.3f, %.3f", m[0][0], m[0][1], m[0][2],
			                                                                                 m[1][0], m[1][1], m[1][2],
			                                                                                 m[2][0], m[2][1], m[2][2]);
	}
}

export using Matrix33  = jpt::TMatrix33<Precision>;
export using Matrix33f = jpt::TMatrix33<float32>;