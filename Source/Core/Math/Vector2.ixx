// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Vector2;

import jpt.Constants;
import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Math;
import jpt.String;
import jpt.ToString;

namespace jpt
{
	export template<Numeric T>
	struct Vector2
	{
	public:
		T x = static_cast<T>(0);
		T y = static_cast<T>(0);

	public:
		static consteval Vector2 Zero()  { return Vector2(static_cast<T>( 0), static_cast<T>(0 )); }
		static consteval Vector2 One()   { return Vector2(static_cast<T>( 1), static_cast<T>(1 )); }
		static consteval Vector2 Up()    { return Vector2(static_cast<T>( 0), static_cast<T>(1 )); }
		static consteval Vector2 Down()  { return Vector2(static_cast<T>( 0), static_cast<T>(-1)); }
		static consteval Vector2 Left()  { return Vector2(static_cast<T>(-1), static_cast<T>(0 )); }
		static consteval Vector2 Right() { return Vector2(static_cast<T>( 1), static_cast<T>(0 )); }

	public:
		constexpr Vector2() = default;
		constexpr Vector2(T x, T y);

		constexpr Vector2 operator+(Vector2 other) const;
		constexpr Vector2 operator-(Vector2 other) const;
		constexpr Vector2 operator*(Vector2 other) const;
		constexpr Vector2 operator/(Vector2 other) const;

		constexpr Vector2& operator+=(Vector2 other);
		constexpr Vector2& operator-=(Vector2 other);
		constexpr Vector2& operator*=(Vector2 other);
		constexpr Vector2& operator/=(Vector2 other);

		constexpr Vector2 operator+(T scalar) const;
		constexpr Vector2 operator-(T scalar) const;
		constexpr Vector2 operator*(T scalar) const;
		constexpr Vector2 operator/(T scalar) const;

		constexpr Vector2& operator+=(T scalar);
		constexpr Vector2& operator-=(T scalar);
		constexpr Vector2& operator*=(T scalar);
		constexpr Vector2& operator/=(T scalar);

		constexpr bool operator==(Vector2 other) const;

		constexpr T Dot(Vector2 other) const;
		constexpr T Length()  const;
		constexpr T Length2() const;
		constexpr T Distance(Vector2 other) const;
		constexpr T Distance2(Vector2 other) const;
		constexpr void Normalize();
		constexpr Vector2 Normalized() const;
		constexpr void Lerp(Vector2 other, T t);
		constexpr Vector2 Lerped(Vector2 other, T t) const;

		constexpr T Angle(Vector2 other) const;	        // Unsigned Radians. Faster (no atan2
		constexpr T AngleDegrees(Vector2 other) const;	
		constexpr T AngleSigned(Vector2 other) const;	// Signed Radians. Slower
		constexpr T AngleSignedDegrees(Vector2 other) const;	
		
		// Counter-clockwise rotation
		constexpr void Rotate(T radians);
		constexpr void RotateAround(Vector2 pivot, T radians);
		constexpr void RotateDegrees(T degrees);
		constexpr void RotateDegreesAround(Vector2 pivot, T degrees);

		constexpr static T Dot(Vector2 left, Vector2 right);
		constexpr static T Length(Vector2 vector);
		constexpr static T Length2(Vector2 vector);
		constexpr static T Distance(Vector2 from, Vector2 to);
		constexpr static T Distance2(Vector2 from, Vector2 to);
		constexpr static Vector2 Normalized(Vector2 vector);
		constexpr static Vector2 Lerp(Vector2 from, Vector2 to, T t);

		constexpr static T Angle(Vector2 lhs, Vector2 rhs);
		constexpr static T AngleDegrees(Vector2 lhs, Vector2 rhs);
		constexpr static T AngleSigned(Vector2 from, Vector2 to);
		constexpr static T AngleSignedDegrees(Vector2 from, Vector2 to);

		constexpr static Vector2 Rotate(Vector2 vec2, T radians);
		constexpr static Vector2 RotateAround(Vector2 vec2, Vector2 pivot, T radians);
		constexpr static Vector2 RotateDegrees(Vector2 vec2, T degrees);
		constexpr static Vector2 RotateDegreesAround(Vector2 vec2, Vector2 pivot, T degrees);

		constexpr jpt::String ToString() const;
	};

	template<Numeric T>
	constexpr Vector2<T> operator+(T scaler, Vector2<T> vector2)
	{
		return vector2 + scaler;
	}

	template<Numeric T>
	constexpr Vector2<T> operator-(T scaler, Vector2<T> vector2)
	{
		return vector2 - scaler;
	}

	template<Numeric T>
	constexpr Vector2<T> operator*(T scaler, Vector2<T> vector2)
	{
		return vector2 * scaler;
	}

	template<Numeric T>
	constexpr Vector2<T> operator/(T scaler, Vector2<T> vector2)
	{
		return vector2 / scaler;
	}

	template<Numeric T>
	constexpr Vector2<T>::Vector2(T x, T y)
		: x(x)
		, y(y)
	{
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::operator+(Vector2 other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::operator-(Vector2 other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::operator*(Vector2 other) const
	{
		return Vector2(x * other.x, y * other.y);
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::operator/(Vector2 other) const
	{
		return Vector2(x / other.x, y / other.y);
	}

	template<Numeric T>
	constexpr Vector2<T>& Vector2<T>::operator+=(Vector2 other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	template<Numeric T>
	constexpr Vector2<T>& Vector2<T>::operator-=(Vector2 other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template<Numeric T>
	constexpr Vector2<T>& Vector2<T>::operator*=(Vector2 other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	template<Numeric T>
	constexpr Vector2<T>& Vector2<T>::operator/=(Vector2 other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::operator+(T scalar) const
	{
		return Vector2(x + scalar, y + scalar);
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::operator-(T scalar) const
	{
		return Vector2(x - scalar, y - scalar);
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::operator*(T scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::operator/(T scalar) const
	{
		return Vector2(x / scalar, y / scalar);
	}

	template<Numeric T>
	constexpr Vector2<T>& Vector2<T>::operator+=(T scalar)
	{
		x += scalar; 
		y += scalar; 
		return *this;
	}

	template<Numeric T>
	constexpr Vector2<T>& Vector2<T>::operator-=(T scalar)
	{
		x -= scalar;
		y -= scalar; 
		return *this;
	}

	template<Numeric T>
	constexpr Vector2<T>& Vector2<T>::operator*=(T scalar)
	{
		x *= scalar; 
		y *= scalar; 
		return *this;
	}

	template<Numeric T>
	constexpr Vector2<T>& Vector2<T>::operator/=(T scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	template<Numeric T>
	constexpr bool Vector2<T>::operator==(Vector2 other) const
	{
		return AreValuesClose(x, other.x, static_cast<T>(0.05)) && 
			   AreValuesClose(y, other.y, static_cast<T>(0.05));
	}

	template<Numeric T>
	constexpr T Vector2<T>::Dot(Vector2 other) const
	{
		return (x * other.x) + (y * other.y);
	}

	template<Numeric T>
	constexpr T Vector2<T>::Length() const
	{
		return std::sqrt(Length2());
	}

	template<Numeric T>
	constexpr T Vector2<T>::Length2() const
	{
		return (x * x) + (y * y);
	}

	template<Numeric T>
	constexpr T Vector2<T>::Distance(Vector2 other) const
	{
		return (*this - other).Length();
	}

	template<Numeric T>
	constexpr T Vector2<T>::Distance2(Vector2 other) const
	{
		return (*this - other).Length2();
	}

	template<Numeric T>
	constexpr void Vector2<T>::Normalize()
	{
		const T length = Length();
		if (length != 0.0f)
		{
			x /= length;
			y /= length;
		}
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::Normalized() const
	{
		Vector2<T> result = *this;
		result.Normalize();
		return result;
	}

	template<Numeric T>
	constexpr void Vector2<T>::Lerp(Vector2 other, T t)
	{
		*this += (other - *this) * t;
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::Lerped(Vector2 other, T t) const
	{
		return *this + (other - *this) * t;
	}

	template<Numeric T>
	constexpr T Vector2<T>::Angle(Vector2 other) const
	{
		const T dot = Dot(other);
		const T angle = std::acos(dot / (Length() * other.Length()));
		return angle;
	}

	template<Numeric T>
	constexpr T Vector2<T>::AngleDegrees(Vector2 other) const
	{
		return ToDegrees(Angle(other));
	}

	template<Numeric T>
	constexpr T Vector2<T>::AngleSigned(Vector2 other) const
	{
		T atan2 = std::atan2(other.y, other.x) - std::atan2(y, x);

		if (atan2 > PI)
		{
			atan2 -= 2 * PI;
		}
		else if (atan2 < -PI)
		{
			atan2 += 2 * PI;
		}

		return atan2;
	}

	template<Numeric T>
	constexpr T Vector2<T>::AngleSignedDegrees(Vector2 other) const
	{
		return ToDegrees(AngleSigned(other));
	}

	template<Numeric T>
	constexpr void Vector2<T>::Rotate(T radians)
	{
		const T cos = std::cos(radians);
		const T sin = std::sin(radians);

		const T dx = x * cos - y * sin;
		const T dy = x * sin + y * cos;

		x = dx;
		y = dy;
	}

	template<Numeric T>
	constexpr void Vector2<T>::RotateAround(Vector2 pivot, T radians)
	{
		const T cos = std::cos(radians);
		const T sin = std::sin(radians);

		const Vector2<T> temp = *this - pivot;

		x = temp.x * cos - temp.y * sin + pivot.x;
		y = temp.x * sin + temp.y * cos + pivot.y;
	}

	template<Numeric T>
	constexpr void Vector2<T>::RotateDegrees(T degrees)
	{
		Rotate(ToRadians(degrees));
	}

	template<Numeric T>
	constexpr void Vector2<T>::RotateDegreesAround(Vector2 pivot, T degrees)
	{
		RotateAround(pivot, ToRadians(degrees));
	}

	template<Numeric T>
	constexpr T Vector2<T>::Dot(Vector2 left, Vector2 right)
	{
		return (left.x * right.x) + (left.y * right.y);
	}

	template<Numeric T>
	constexpr T Vector2<T>::Length(Vector2 vector)
	{
		return vector.Length();
	}

	template<Numeric T>
	constexpr T Vector2<T>::Length2(Vector2 vector)
	{
		return vector.Length2();
	}

	template<Numeric T>
	constexpr T Vector2<T>::Distance(Vector2 from, Vector2 to)
	{
		return from.Distance(to);
	}

	template<Numeric T>
	constexpr T Vector2<T>::Distance2(Vector2 from, Vector2 to)
	{
		return from.Distance2(to);
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::Normalized(Vector2 vector)
	{
		return vector.Normalized();
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::Lerp(Vector2 from, Vector2 to, T t)
	{
		return from.Lerped(to, t);
	}

	template<Numeric T>
	constexpr T Vector2<T>::Angle(Vector2 lhs, Vector2 rhs)
	{
		return lhs.Angle(rhs);
	}

	template<Numeric T>
	constexpr T Vector2<T>::AngleDegrees(Vector2 lhs, Vector2 rhs)
	{
		return lhs.AngleDegrees(rhs);
	}

	template<Numeric T>
	constexpr T Vector2<T>::AngleSigned(Vector2 from, Vector2 to)
	{
		return from.AngleSigned(to);
	}

	template<Numeric T>
	constexpr T Vector2<T>::AngleSignedDegrees(Vector2 from, Vector2 to)
	{
		return from.AngleSignedDegrees(to);
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::Rotate(Vector2 vec2, T radians)
	{
		vec2.Rotate(radians);
		return vec2;
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::RotateAround(Vector2 vec2, Vector2 pivot, T radians)
	{
		vec2.RotateAround(pivot, radians);
		return vec2;
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::RotateDegrees(Vector2 vec2, T degrees)
	{
		vec2.RotateDegrees(degrees);
		return vec2;
	}

	template<Numeric T>
	constexpr Vector2<T> Vector2<T>::RotateDegreesAround(Vector2 vec2, Vector2 pivot, T degrees)
	{
		vec2.RotateDegreesAround(pivot, degrees);
		return vec2;
	}

	template<Numeric T>
	constexpr jpt::String Vector2<T>::ToString() const
	{
		jpt::String result;
		result += "x: ";
		result += jpt::ToString(x);
		result += ", y:";
		result += jpt::ToString(y);
		return result;
	}
}

export using Vec2f = jpt::Vector2<float>;
export using Vec2d = jpt::Vector2<double>;
export using Vec2i = jpt::Vector2<int32>;
export using Vec2u = jpt::Vector2<uint32>;

template<> constexpr bool jpt::IsTrivial<Vec2f> = true;
template<> constexpr bool jpt::IsTrivial<Vec2d> = true;
template<> constexpr bool jpt::IsTrivial<Vec2i> = true;
template<> constexpr bool jpt::IsTrivial<Vec2u> = true;