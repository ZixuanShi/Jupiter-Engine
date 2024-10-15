// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Vector3;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Math;
import jpt.String;
import jpt.ToString;

export namespace jpt
{
	template<Numeric T>
	struct Vector3
	{
	public:
		T x = static_cast<T>(0);
		T y = static_cast<T>(0);
		T z = static_cast<T>(0);

	public:
		static consteval Vector3 Zero()     { return Vector3(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>( 0)); }
		static consteval Vector3 One()      { return Vector3(static_cast<T>( 1), static_cast<T>( 1), static_cast<T>( 1)); }
		static consteval Vector3 Up()       { return Vector3(static_cast<T>( 0), static_cast<T>( 1), static_cast<T>( 0)); }
		static consteval Vector3 Down()     { return Vector3(static_cast<T>( 0), static_cast<T>(-1), static_cast<T>( 0)); }
		static consteval Vector3 Left()     { return Vector3(static_cast<T>(-1), static_cast<T>( 0), static_cast<T>( 0)); }
		static consteval Vector3 Right()    { return Vector3(static_cast<T>( 1), static_cast<T>( 0), static_cast<T>( 0)); }
		static consteval Vector3 Forward()  { return Vector3(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>( 1)); }
		static consteval Vector3 Backward() { return Vector3(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>(-1)); }

	public:
		constexpr Vector3() = default;
		constexpr Vector3(T scalar);
		constexpr Vector3(T _x, T _y, T _z);

		constexpr Vector3 operator-() const;

		constexpr Vector3 operator+(const Vector3& other) const;
		constexpr Vector3 operator-(const Vector3& other) const;
		constexpr Vector3 operator*(const Vector3& other) const;
		constexpr Vector3 operator/(const Vector3& other) const;

		constexpr Vector3& operator+=(const Vector3& other);
		constexpr Vector3& operator-=(const Vector3& other);
		constexpr Vector3& operator*=(const Vector3& other);
		constexpr Vector3& operator/=(const Vector3& other);

		constexpr Vector3 operator+(T scalar) const;
		constexpr Vector3 operator-(T scalar) const;
		constexpr Vector3 operator*(T scalar) const;
		constexpr Vector3 operator/(T scalar) const;

		constexpr Vector3& operator+=(T scalar);
		constexpr Vector3& operator-=(T scalar);
		constexpr Vector3& operator*=(T scalar);
		constexpr Vector3& operator/=(T scalar);

		constexpr T& operator[](size_t index) { return (&x)[index]; }
		constexpr const T& operator[](size_t index) const { return (&x)[index]; }

		constexpr bool operator==(const Vector3& other) const;

		constexpr T Dot(const Vector3& other) const;
		constexpr Vector3 Cross(const Vector3& other) const;
		constexpr T Length() const;
		constexpr T Length2() const;
		constexpr T Distance(const Vector3& other) const;
		constexpr T Distance2(const Vector3& other) const;
		constexpr void Normalize();
		constexpr Vector3 Normalized() const;
		constexpr T Angle(const Vector3& other) const;
		constexpr Vector3 Lerp(const Vector3& other, T t) const;
		constexpr Vector3 InvLerp(const Vector3& other, const Vector3& value) const;
		constexpr Vector3 RotateAxis(const Vector3& axis, T radians) const;
		constexpr Vector3 RotatePoint(const Vector3& point, const Vector3& axis, T radians) const;

		constexpr static T Dot(const Vector3& left, const Vector3&right);
		constexpr static Vector3 Cross(const Vector3& left, const Vector3&right);
		constexpr static T Length(const Vector3& vector3);
		constexpr static T Length2(const Vector3& vector3);
		constexpr static T Distance(const Vector3& left, const Vector3&right);
		constexpr static T Distance2(const Vector3& left, const Vector3&right);
		constexpr static Vector3 Normalize(const Vector3& vector3);
		constexpr static T Angle(const Vector3& left, const Vector3&right);
		constexpr static Vector3 Lerp(const Vector3& start, const Vector3&end, T t);
		constexpr static Vector3 InvLerp(const Vector3& start, const Vector3&end, const Vector3&value);
		constexpr static Vector3 Project(const Vector3& vector3, const Vector3& normal);

		constexpr String ToString() const;
	};

	// ------------------------------------------------------------------------------------------------
	// Member functions
	// ------------------------------------------------------------------------------------------------
	template<Numeric T>
	constexpr Vector3<T>::Vector3(T scalar)
		: x(scalar)
		, y(scalar)
		, z(scalar)
	{
	}

	template<Numeric T>
	constexpr Vector3<T>::Vector3(T _x, T _y, T _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::operator*(const Vector3& other) const
	{
		return Vector3(x * other.x, y * other.y, z * other.z);
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::operator/(const Vector3& other) const
	{
		return Vector3(x / other.x, y / other.y, z / other.z);
	}

	template<Numeric T>
	constexpr Vector3<T>& Vector3<T>::operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	template<Numeric T>
	constexpr Vector3<T>& Vector3<T>::operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	template<Numeric T>
	constexpr Vector3<T>& Vector3<T>::operator*=(const Vector3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	template<Numeric T>
	constexpr Vector3<T>& Vector3<T>::operator/=(const Vector3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::operator+(T scalar) const
	{
		return Vector3(x + scalar, y + scalar, z + scalar);
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::operator-(T scalar) const
	{
		return Vector3(x - scalar, y - scalar, z - scalar);
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::operator*(T scalar) const
	{
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::operator/(T scalar) const
	{
		return Vector3(x / scalar, y / scalar, z / scalar);
	}

	template<Numeric T>
	constexpr Vector3<T>& Vector3<T>::operator+=(T scalar)
	{
		x += scalar;
		y += scalar;
		z += scalar;
		return *this;
	}

	template<Numeric T>
	constexpr Vector3<T>& Vector3<T>::operator-=(T scalar)
	{
		x -= scalar;
		y -= scalar;
		z -= scalar;
		return *this;
	}

	template<Numeric T>
	constexpr Vector3<T>& Vector3<T>::operator*=(T scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	template<Numeric T>
	constexpr Vector3<T>& Vector3<T>::operator/=(T scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	template<Numeric T>
	constexpr bool Vector3<T>::operator==(const Vector3& other) const
	{
		return AreValuesClose(x, other.x, static_cast<T>(0.05)) && 
			   AreValuesClose(y, other.y, static_cast<T>(0.05)) && 
			   AreValuesClose(z, other.z, static_cast<T>(0.05));
	}

	template<Numeric T>
	constexpr T Vector3<T>::Dot(const Vector3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::Cross(const Vector3& other) const
	{
		return Vector3(y * other.z - z * other.y, 
			           z * other.x - x * other.z, 
			           x * other.y - y * other.x);
	}

	template<Numeric T>
	constexpr T Vector3<T>::Length() const
	{
		return std::sqrt(Length2());
	}

	template<Numeric T>
	constexpr T Vector3<T>::Length2() const
	{
		return x * x + y * y + z * z;
	}

	template<Numeric T>
	constexpr T Vector3<T>::Distance(const Vector3& other) const
	{
		return std::sqrt(Distance2(other));
	}

	template<Numeric T>
	constexpr T Vector3<T>::Distance2(const Vector3& other) const
	{
		return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z);
	}

	template<Numeric T>
	constexpr void Vector3<T>::Normalize()
	{
		const T length = Length();
		if (length > static_cast<T>(0))
		{
			x /= length;
			y /= length;
			z /= length;
		}
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::Normalized() const
	{
		Vector3<T> result = *this;
		result.Normalize();
		return result;
	}

	template<Numeric T>
	constexpr T Vector3<T>::Angle(const Vector3& other) const
	{
		return std::acos(Dot(other) / (Length() * other.Length()));
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::Lerp(const Vector3& other, T t) const
	{
		return *this + (other - *this) * t;
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::InvLerp(const Vector3& other, const Vector3&value) const
	{
		return (value - *this) / (other - *this);
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::RotateAxis(const Vector3& axis, T radians) const
	{
		// Rotates a position vector around an axis by a given angle
		// https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula

		const T cosTheta = std::cos(radians);
		const T sinTheta = std::sin(radians);

		const Vector3<T> cross = axis.Cross(*this);
		const Vector3<T> crossCross = axis.Cross(cross);

		return *this * cosTheta + cross * sinTheta + crossCross * (static_cast<T>(1) - cosTheta);
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::RotatePoint(const Vector3& point, const Vector3& axis, T radians) const
	{
		// Rotates this position vector around a point by a given angle around a specified axis
		const Vector3<T> offset = *this - point;
		const Vector3<T> rotated = offset.RotateAxis(axis, radians);
		return rotated + point;
	}

	template<Numeric T>
	constexpr T Vector3<T>::Dot(const Vector3& left, const Vector3&right)
	{
		return left.Dot(right);
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::Cross(const Vector3& left, const Vector3&right)
	{
		return left.Cross(right);
	}

	template<Numeric T>
	constexpr T Vector3<T>::Length(const Vector3& vector3)
	{
		return vector3.Length();
	}

	template<Numeric T>
	constexpr T Vector3<T>::Length2(const Vector3& vector3)
	{
		return vector3.Length2();
	}

	template<Numeric T>
	constexpr T Vector3<T>::Distance(const Vector3& left, const Vector3&right)
	{
		return left.Distance(right);
	}

	template<Numeric T>
	constexpr T Vector3<T>::Distance2(const Vector3& left, const Vector3&right)
	{
		return left.Distance2(right);
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::Normalize(const Vector3& vector3)
	{
		Vector3<T> result = vector3;
		result.Normalize();
		return result;
	}

	template<Numeric T>
	constexpr T Vector3<T>::Angle(const Vector3& left, const Vector3&right)
	{
		return left.Angle(right);
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::Lerp(const Vector3& start, const Vector3&end, T t)
	{
		return start.Lerp(end, t);
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::InvLerp(const Vector3& start, const Vector3&end, const Vector3&value)
	{
		return start.InvLerp(end, value);
	}

	template<Numeric T>
	constexpr Vector3<T> Vector3<T>::Project(const Vector3& vector3, const Vector3& normal)
	{
		// projection = v - (v · n) / (n · n) * n

		const T dotProduct = vector3.Dot(normal);
		const T normalLengthSquared = normal.Dot(normal);
		return vector3 - normal * (dotProduct / normalLengthSquared);
	}

	template<Numeric T>
	constexpr String Vector3<T>::ToString() const
	{
		return String::Format<64>("x: %.3f, y: %.3f, w: %.3f", x, y, z);
	}
}

export using Vec3f = jpt::Vector3<float>;
export using Vec3d = jpt::Vector3<double>;
export using Vec3i = jpt::Vector3<int32>;
export using Vec3u = jpt::Vector3<uint32>;

template<> constexpr bool jpt::IsTrivial<Vec3f> = true;
template<> constexpr bool jpt::IsTrivial<Vec3i> = true;
template<> constexpr bool jpt::IsTrivial<Vec3u> = true;

export template<jpt::Numeric T>
constexpr jpt::Vector3<T> operator+(T scaler, const jpt::Vector3<T>& vector3)
{
	return vector3 + scaler;
}

export template<jpt::Numeric T>
constexpr jpt::Vector3<T> operator-(T scaler, const jpt::Vector3<T>& vector3)
{
	return vector3 - scaler;
}

export template<jpt::Numeric T>
constexpr jpt::Vector3<T> operator*(T scaler, const jpt::Vector3<T>& vector3)
{
	return vector3 * scaler;
}

export template<jpt::Numeric T>
constexpr jpt::Vector3<T> operator/(T scaler, const jpt::Vector3<T>& vector3)
{
	return vector3 / scaler;
}