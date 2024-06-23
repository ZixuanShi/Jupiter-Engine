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
		constexpr Vector4() = default;
		constexpr Vector4(T scalar);
		constexpr Vector4(T x, T y, T z, T w);
		constexpr Vector4(const Vector3<T>& vector, T w);

		constexpr T Dot(const Vector4& other) const;
		constexpr T Length() const;
		constexpr T Length2() const;
		constexpr void Normalize();

		constexpr static T Dot(const Vector4& lhs, const Vector4& rhs);
		constexpr static Vector4 Normalized(const Vector4& vector);

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

		constexpr bool operator==(const Vector4& other) const;

		constexpr T& operator[](size_t index) { return (&x)[index]; }
		constexpr const T& operator[](size_t index) const { return (&x)[index]; }
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
	constexpr T Vector4<T>::Dot(const Vector4& other) const
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	template<Numeric T>
	constexpr T Vector4<T>::Length() const
	{
		return std::sqrt(x * x + y * y + z * z + w * w);
	}

	template<Numeric T>
	constexpr T Vector4<T>::Length2() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template<Numeric T>
	constexpr void Vector4<T>::Normalize()
	{
		const T length = Length(*this);
		if (length > static_cast<T>(0))
		{
			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}
	}

	template<Numeric T>
	constexpr T Vector4<T>::Dot(const Vector4& lhs, const Vector4& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	template<Numeric T>
	constexpr Vector4<T> Vector4<T>::Normalized(const Vector4& vector)
	{
		Vector4 result = vector;
		result.Normalize();
		return result;
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
	constexpr bool Vector4<T>::operator==(const Vector4& other) const
	{
		return AreValuesClose(x, other.x, static_cast<T>(0.05)) && 
			   AreValuesClose(y, other.y, static_cast<T>(0.05)) && 
			   AreValuesClose(z, other.z, static_cast<T>(0.05)) && 
			   AreValuesClose(w, other.w, static_cast<T>(0.05));
	}
}

export using Vec4f = jpt::Vector4<float32>;