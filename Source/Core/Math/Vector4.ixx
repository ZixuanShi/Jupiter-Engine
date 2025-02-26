// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Vector4;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Math;
import jpt.String;
import jpt.Vector3;
import jpt.Hash;

export namespace jpt
{
	template<Numeric T>
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

	public:
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

		constexpr Vector4& operator+=(T scalar);
		constexpr Vector4& operator-=(T scalar);
		constexpr Vector4& operator*=(T scalar);
		constexpr Vector4& operator/=(T scalar);

		constexpr T& operator[](size_t index) noexcept;
		constexpr const T& operator[](size_t index) const noexcept;

	public:
		constexpr T Length2() const;
		constexpr T Length2W() const;
		constexpr T Length() const;
		constexpr T LengthW() const;

		constexpr T Dot(const Vector4& other) const;
		constexpr T DotW(const Vector4& other) const;
		constexpr Vector3<T> Cross(const Vector4& other) const;

		// Homogeneous coordinates
		constexpr bool IsDir() const;
		constexpr bool IsPos() const;

		constexpr Vector3<T> XYZ() const;
	};

	// ------------------------------------------------------------------------------------------------
	// Non-Member functions
	// ------------------------------------------------------------------------------------------------
	template<Numeric T>
	constexpr bool operator==(const Vector4<T>& lhs, const Vector4<T>& rhs)
	{
		return AreValuesClose(lhs.x, rhs.x, static_cast<T>(0.05)) &&
			AreValuesClose(lhs.y, rhs.y, static_cast<T>(0.05)) &&
			AreValuesClose(lhs.z, rhs.z, static_cast<T>(0.05)) &&
			AreValuesClose(lhs.w, rhs.w, static_cast<T>(0.05));
	}

	template<Numeric T>
	constexpr String ToString(const Vector4<T>& vector4)
	{
		return String::Format<64>("x: %.3f, y: %.3f, w: %.3f, w: %.3f", vector4.x, vector4.y, vector4.z, vector4.w);
	}

	template<Numeric T>
	constexpr uint64 Hash(const Vector4<T>& vector4)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			const T epsilon = static_cast<T>(1e-6);
			auto round = [epsilon](T value) -> T
				{
					return (value < epsilon && value > -epsilon) ? static_cast<T>(0) : value;
				};

			uint64 hash = jpt::Hash(round(vector4.x));
			hash ^= jpt::Hash(round(vector4.y)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= jpt::Hash(round(vector4.z)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= jpt::Hash(round(vector4.w)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			return hash;
		}
		else
		{
			uint64 hash = jpt::Hash(vector4.x);
			hash ^= jpt::Hash(vector4.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= jpt::Hash(vector4.z) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= jpt::Hash(vector4.w) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			return hash;
		}
	}

	// ------------------------------------------------------------------------------------------------
	// Member functions
	// ------------------------------------------------------------------------------------------------
	template<Numeric T>
	constexpr Vector4<T>::Vector4(T scalar)
		: x(scalar)
		, y(scalar)
		, z(scalar)
		, w(static_cast<T>(1)) 
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
	constexpr Vector4<T>& Vector4<T>::operator+=(T scalar)
	{
		x += scalar;
		y += scalar;
		z += scalar;
		w += scalar;
		return *this;
	}

	template<Numeric T>
	constexpr Vector4<T>& Vector4<T>::operator-=(T scalar)
	{
		x -= scalar;
		y -= scalar;
		z -= scalar;
		w -= scalar;
		return *this;
	}

	template<Numeric T>
	constexpr Vector4<T>& Vector4<T>::operator*=(T scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	template<Numeric T>
	constexpr Vector4<T>& Vector4<T>::operator/=(T scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	template<Numeric T>
	constexpr T& Vector4<T>::operator[](size_t index) noexcept
	{
		return (&x)[index];
	}

	template<Numeric T>
	constexpr const T& Vector4<T>::operator[](size_t index) const noexcept
	{
		return (&x)[index];
	}

	template<Numeric T>
	constexpr T Vector4<T>::Length2() const
	{
		return x * x + y * y + z * z;
	}

	template<Numeric T>
	constexpr T Vector4<T>::Length2W() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template<Numeric T>
	constexpr T Vector4<T>::Length() const
	{
		return std::sqrt(Length2());
	}

	template<Numeric T>
	constexpr T Vector4<T>::LengthW() const
	{
		return std::sqrt(Length2W());
	}

	template<Numeric T>
	constexpr T Vector4<T>::Dot(const Vector4& other) const
	{
		return XYZ().Dot(other.XYZ());
	}

	template<Numeric T>
	constexpr T Vector4<T>::DotW(const Vector4& other) const
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	template<Numeric T>
	constexpr Vector3<T> Vector4<T>::Cross(const Vector4& other) const
	{
		return XYZ().Cross(other.XYZ());
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
	constexpr Vector3<T> Vector4<T>::XYZ() const
	{
		return Vector3<T>(x, y, z);
	}
}

export using Vec4  = jpt::Vector4<Precision>;
export using Vec4f = jpt::Vector4<float32>;
export using Vec4d = jpt::Vector4<float64>;
export using Vec4i = jpt::Vector4<int32>;

template<> constexpr bool jpt::IsTrivial<Vec4f> = true;
template<> constexpr bool jpt::IsTrivial<Vec4i> = true;

export template<jpt::Numeric T>
constexpr jpt::Vector4<T> operator+(T scaler, const jpt::Vector4<T>& vector4)
{
	return vector4 + scaler;
}

export template<jpt::Numeric T>
constexpr jpt::Vector4<T> operator-(T scaler, const jpt::Vector4<T>& vector4)
{
	return vector4 - scaler;
}

export template<jpt::Numeric T>
constexpr jpt::Vector4<T> operator*(T scaler, const jpt::Vector4<T>& vector4)
{
	return vector4 * scaler;
}

export template<jpt::Numeric T>
constexpr jpt::Vector4<T> operator/(T scaler, const jpt::Vector4<T>& vector4)
{
	return vector4 / scaler;
}