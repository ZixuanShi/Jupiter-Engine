// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Vector2;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.String;
import jpt.ToString;

export namespace jpt
{
	template<Numeric T>
	struct Vector2
	{
		T x = static_cast<T>(0);
		T y = static_cast<T>(0);

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

		constexpr static T Dot(Vector2 left, Vector2 right);
		constexpr static T Length(Vector2 vector);
		constexpr static T Length2(Vector2 vector);
		constexpr static T Distance(Vector2 left, Vector2 right);
		constexpr static T Distance2(Vector2 left, Vector2 right);

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
		return x == other.x && y == other.y;
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
	constexpr T Vector2<T>::Distance(Vector2 left, Vector2 right)
	{
		return left.Distance(right);
	}

	template<Numeric T>
	constexpr T Vector2<T>::Distance2(Vector2 left, Vector2 right)
	{
		return left.Distance2(right);
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

	using Vec2f = Vector2<float>;
	using Vec2d = Vector2<double>;
	using Vec2i = Vector2<int32>;
	using Vec2u = Vector2<uint32>;
}