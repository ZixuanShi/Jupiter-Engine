// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Vector4;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Math;
import jpt.String;
import jpt.ToString;
import jpt.Vector3;

namespace jpt
{
	export template<Numeric T>
	struct Vector4
	{
	public:
		T x = static_cast<T>(0);
		T y = static_cast<T>(0);
		T z = static_cast<T>(0);
		T w = static_cast<T>(0);

	public:
		static consteval Vector4 Zero()     { return Vector4(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>( 0), static_cast<T>(0)); }
		static consteval Vector4 One()      { return Vector4(static_cast<T>( 1), static_cast<T>( 1), static_cast<T>( 1), static_cast<T>(1)); }
		static consteval Vector4 Up()       { return Vector4(static_cast<T>( 0), static_cast<T>( 1), static_cast<T>( 0), static_cast<T>(0)); }
		static consteval Vector4 Down()     { return Vector4(static_cast<T>( 0), static_cast<T>(-1), static_cast<T>( 0), static_cast<T>(0)); }
		static consteval Vector4 Left()     { return Vector4(static_cast<T>(-1), static_cast<T>( 0), static_cast<T>( 0), static_cast<T>(0)); }
		static consteval Vector4 Right()    { return Vector4(static_cast<T>( 1), static_cast<T>( 0), static_cast<T>( 0), static_cast<T>(0)); }
		static consteval Vector4 Forward()  { return Vector4(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>( 1), static_cast<T>(0)); }
		static consteval Vector4 Backward() { return Vector4(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>(-1), static_cast<T>(0)); }

	public:
		constexpr Vector4() = default;
		constexpr Vector4(T scalar);
		constexpr Vector4(T x, T y, T z, T w);
		constexpr Vector4(const Vector3<T>& vector, T w);

		constexpr Vector4 operator-() const;

		constexpr Vector4 operator+(const Vector4& other) const;
		constexpr Vector4 operator-(const Vector4& other) const;
		constexpr Vector4 operator*(const Vector4& other) const;
		constexpr Vector4 operator/(const Vector4& other) const;

		constexpr Vector4& operator+=(const Vector4& other);
		constexpr Vector4& operator-=(const Vector4& other);
		constexpr Vector4& operator*=(const Vector4& other);
		constexpr Vector4& operator/=(const Vector4& other);

		constexpr Vector4 operator+(T scalar) const;
		constexpr Vector4 operator-(T scalar) const;
		constexpr Vector4 operator*(T scalar) const;
		constexpr Vector4 operator/(T scalar) const;

		constexpr Vector4& operator+=(T scalar) { return *this = *this + scalar; }
		constexpr Vector4& operator-=(T scalar) { return *this = *this - scalar; }
		constexpr Vector4& operator*=(T scalar) { return *this = *this * scalar; }
		constexpr Vector4& operator/=(T scalar) { return *this = *this / scalar; }

		constexpr T& operator[](size_t index) { return (&x)[index]; }
		constexpr const T& operator[](size_t index) const { return (&x)[index]; }

		constexpr bool operator==(const Vector4& other) const;

		// Homogeneous coordinates
		constexpr bool IsDir() const;
		constexpr bool IsPos() const;
	};

	template<Numeric T>
	constexpr Vector4<T>::Vector4(T scalar)
		: x(scalar)
		, y(scalar)
		, z(scalar)
		, w(scalar) 
	{
	}

	template<Numeric T>
	constexpr Vector4<T>::Vector4(T x, T y, T z, T w)
		: x(x)
		, y(y)
		, z(z)
		, w(w) 
	{
	}

	template<Numeric T>
	constexpr Vector4<T>::Vector4(const Vector3<T>& vector, T w)
		: x(vector.x)
		, y(vector.y)
		, z(vector.z)
		, w(w) 
	{
	}

	template<Numeric T>
	constexpr Vector4<T> Vector4<T>::operator-() const
	{
		return Vector4(-x, -y, -z, -w);
	}

	template<Numeric T>
	constexpr Vector4<T> Vector4<T>::operator+(const Vector4& other) const
	{
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	template<Numeric T>
	constexpr Vector4<T> Vector4<T>::operator-(const Vector4& other) const
	{
		return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	template<Numeric T>
	constexpr Vector4<T> Vector4<T>::operator*(const Vector4& other) const
	{
		return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
	}

	template<Numeric T>
	constexpr Vector4<T> Vector4<T>::operator/(const Vector4& other) const
	{
		return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
	}

	template<Numeric T>
	constexpr Vector4<T>& Vector4<T>::operator+=(const Vector4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	template<Numeric T>
	constexpr Vector4<T>& Vector4<T>::operator-=(const Vector4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	template<Numeric T>
	constexpr Vector4<T>& Vector4<T>::operator*=(const Vector4& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}

	template<Numeric T>
	constexpr Vector4<T>& Vector4<T>::operator/=(const Vector4& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}

	template<Numeric T>
	constexpr Vector4<T> Vector4<T>::operator+(T scalar) const
	{
		return Vector4(x + scalar, y + scalar, z + scalar, w + scalar);
	}

	template<Numeric T>
	constexpr Vector4<T> Vector4<T>::operator-(T scalar) const
	{
		return Vector4(x - scalar, y - scalar, z - scalar, w - scalar);
	}

	template<Numeric T>
	constexpr Vector4<T> Vector4<T>::operator*(T scalar) const
	{
		return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	template<Numeric T>
	constexpr Vector4<T> Vector4<T>::operator/(T scalar) const
	{
		return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	template<Numeric T>
	constexpr bool Vector4<T>::IsDir() const
	{
		return w == static_cast<T>(0);
	}

	template<Numeric T>
	constexpr bool Vector4<T>::IsPos() const
	{
		return w > static_cast<T>(0);
	}

	template<Numeric T>
	constexpr bool Vector4<T>::operator==(const Vector4& other) const
	{
		return AreValuesClose(x, other.x, static_cast<T>(0.05)) && 
			   AreValuesClose(y, other.y, static_cast<T>(0.05)) && 
			   AreValuesClose(z, other.z, static_cast<T>(0.05)) && 
			   AreValuesClose(w, other.w, static_cast<T>(0.05));
	}
}

export using Vec4f = jpt::Vector4<float32>;