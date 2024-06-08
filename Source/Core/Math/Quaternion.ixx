// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Quaternion;

import jpt.Concepts;
import jpt.Constants;
import jpt.TypeDefs;
import jpt.Vector3;

namespace jpt
{
	/** A quaternion is a 4D vector that represents a rotation in 3D space.
		The first three components are the axis of rotation, and the fourth component is the angle of rotation.
		The quaternion is normalized so that the first three components form a unit vector.
		The angle is in radians.
		Bye Gimbal-Lock */
	export template<Numeric T>
	struct TQuaternion
	{
	public:
		T x = static_cast<T>(0);
		T y = static_cast<T>(0);
		T z = static_cast<T>(0);
		T w = static_cast<T>(1);	/**< Radians */

	public:
		static constexpr TQuaternion Identity() { return TQuaternion(); }

	public:
		constexpr TQuaternion() = default;
		constexpr TQuaternion(T x, T y, T z, T w);
		constexpr TQuaternion(const Vector3<T>& axis);
		constexpr TQuaternion(const Vector3<T>& axis, T radians);

		constexpr TQuaternion operator*(const TQuaternion& rhs) const;
		constexpr TQuaternion operator+(const TQuaternion& rhs) const;
		constexpr TQuaternion operator*(T scalar) const;
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
	constexpr TQuaternion<T> TQuaternion<T>::operator*(const TQuaternion& rhs) const
	{
		const T w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		const T x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
		const T y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
		const T z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
		return TQuaternion(x, y, z, w);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::operator*(T scalar) const
	{
		return TQuaternion(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::operator+(const TQuaternion& rhs) const
	{ 
		return TQuaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); 
	}
}