// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.EulerAngles;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Math;
import jpt.Vector3;
import jpt.String;
import jpt.ToString;

namespace jpt
{
	export template<Numeric T>
	struct EulerAngles
	{
	public:
		T yaw   = static_cast<T>(0);
		T pitch = static_cast<T>(0);
		T roll  = static_cast<T>(0);

	public:
		constexpr EulerAngles() = default;
		constexpr EulerAngles(T yaw, T pitch, T roll);
		constexpr EulerAngles(const Vector3<T>& v);

		constexpr EulerAngles<T> operator+(const EulerAngles<T>& rhs) const;
		constexpr EulerAngles<T> operator-(const EulerAngles<T>& rhs) const;
		constexpr EulerAngles<T> operator*(T scalar) const;
		constexpr EulerAngles<T> operator/(T scalar) const;
	};

	template<Numeric T>
	constexpr EulerAngles<T>::EulerAngles(T yaw, T pitch, T roll)
		: yaw(yaw)
		, pitch(pitch)
		, roll(roll) 
	{
	}

	template<Numeric T>
	constexpr EulerAngles<T>::EulerAngles(const Vector3<T>& v)
		: yaw(v.x)
		, pitch(v.y)
		, roll(v.z)
	{
	}

	template<Numeric T>
	constexpr EulerAngles<T> EulerAngles<T>::operator+(const EulerAngles<T>& rhs) const
	{
		const T newYaw = yaw + rhs.yaw;
		const T newPitch = pitch + rhs.pitch;
		const T newRoll = roll + rhs.roll;
		return EulerAngles<T>(newYaw, newPitch, newRoll);
	}

	template<Numeric T>
	constexpr EulerAngles<T> EulerAngles<T>::operator-(const EulerAngles<T>& rhs) const
	{
		const T newYaw = yaw - rhs.yaw;
		const T newPitch = pitch - rhs.pitch;
		const T newRoll = roll - rhs.roll;
		return EulerAngles<T>(newYaw, newPitch, newRoll);
	}

	template<Numeric T>
	constexpr EulerAngles<T> EulerAngles<T>::operator*(T scalar) const
	{
		const T newYaw = yaw * scalar;
		const T newPitch = pitch * scalar;
		const T newRoll = roll * scalar;
		return EulerAngles<T>(newYaw, newPitch, newRoll);
	}

	template<Numeric T>
	constexpr EulerAngles<T> EulerAngles<T>::operator/(T scalar) const
	{
		const T newYaw = yaw / scalar;
		const T newPitch = pitch / scalar;
		const T newRoll = roll / scalar;
		return EulerAngles<T>(newYaw, newPitch, newRoll);
	}
}

export using EulerAnglesf = jpt::EulerAngles<float32>;