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
		The angle is in radians. */
	export template<Numeric T>
	struct TQuaternion
	{
	public:
		T x = static_cast<T>(0);
		T y = static_cast<T>(0);
		T z = static_cast<T>(0);
		T w = static_cast<T>(1);	/**< Radians */

	public:
		constexpr TQuaternion() = default;
		constexpr TQuaternion(T x, T y, T z, T w);
		constexpr TQuaternion(const Vector3<T>& axis);
		constexpr TQuaternion(const Vector3<T>& axis, T radians);
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
}