// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Quaternion;

import jpt.Concepts;
import jpt.Constants;
import jpt.Math;
import jpt.TypeDefs;
import jpt.Vector3;

namespace jpt
{
	export template<Numeric T>
	struct TQuaternion
	{
	public:
		T x = static_cast<T>(0);
		T y = static_cast<T>(0);
		T z = static_cast<T>(0);
		T w = static_cast<T>(1);

	public:
		static constexpr TQuaternion Identity() { return TQuaternion(); }

	public:
		constexpr TQuaternion() = default;
		constexpr TQuaternion(T x, T y, T z, T w);
		constexpr TQuaternion(const Vector3<T>& axis);
		constexpr TQuaternion(const Vector3<T>& axis, T radians);

		constexpr TQuaternion operator+(const TQuaternion& rhs) const;
		constexpr TQuaternion operator-(const TQuaternion& rhs) const;
		constexpr TQuaternion operator*(const TQuaternion& rhs) const;
		constexpr TQuaternion operator/(const TQuaternion& rhs) const;

		constexpr TQuaternion operator/(T scalar) const;
		constexpr TQuaternion operator*(T scalar) const;

		constexpr TQuaternion& operator*=(const TQuaternion& rhs);
		constexpr TQuaternion& operator+=(const TQuaternion& rhs);

		constexpr bool operator==(const TQuaternion& rhs) const;
	};

	template<Numeric T>
	constexpr TQuaternion<T>::TQuaternion(T x, T y, T z, T w)
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{
	}

	template<Numeric T>
	constexpr TQuaternion<T>::TQuaternion(const Vector3<T>& axis)
		: x(axis.x)
		, y(axis.y)
		, z(axis.z)
		, w(static_cast<T>(1))
	{
	}

	template<Numeric T>
	constexpr TQuaternion<T>::TQuaternion(const Vector3<T>& axis, T radians)
		: x(axis.x)
		, y(axis.y)
		, z(axis.z)
		, w(radians)
	{
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::operator+(const TQuaternion& rhs) const
	{
		return TQuaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::operator-(const TQuaternion& rhs) const
	{
		return TQuaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::operator*(const TQuaternion& rhs) const
	{
		const T w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		const T x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
		const T y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
		const T z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
		return TQuaternion(x, y, z, w);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::operator/(const TQuaternion& rhs) const
	{
		return TQuaternion(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::operator/(T scalar) const
	{
		return TQuaternion(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::operator*(T scalar) const
	{
		return TQuaternion(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	template<Numeric T>
	constexpr TQuaternion<T>& TQuaternion<T>::operator*=(const TQuaternion& rhs)
	{
		return *this = *this * rhs;
	}

	template<Numeric T>
	constexpr TQuaternion<T>& TQuaternion<T>::operator+=(const TQuaternion& rhs)
	{
		return *this = *this + rhs;
	}

	template<Numeric T>
	constexpr bool TQuaternion<T>::operator==(const TQuaternion& rhs) const
	{
		return AreValuesClose(x, rhs.x) &&
			AreValuesClose(y, rhs.y) &&
			AreValuesClose(z, rhs.z) &&
			AreValuesClose(w, rhs.w);
	}
}

export using Quaternionf = jpt::TQuaternion<float>;
export using Quaterniond = jpt::TQuaternion<float>;