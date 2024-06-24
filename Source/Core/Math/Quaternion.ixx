// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Quaternion;

import jpt.Concepts;
import jpt.Constants;
import jpt.Math;
import jpt.TypeDefs;
import jpt.Vector3;
import jpt.Matrix33;
import jpt.Matrix44;
import jpt.String;
import jpt.ToString;

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
		static const TQuaternion<T> Identity;

	public:
		constexpr TQuaternion() = default;
		constexpr TQuaternion(T x, T y, T z, T w);

		constexpr TQuaternion operator+(const TQuaternion& rhs) const;
		constexpr TQuaternion operator-(const TQuaternion& rhs) const;
		constexpr TQuaternion operator*(const TQuaternion& rhs) const;
		constexpr TQuaternion operator/(const TQuaternion& rhs) const;

		constexpr TQuaternion operator/(T scalar) const;
		constexpr TQuaternion operator*(T scalar) const;

		constexpr TQuaternion& operator*=(const TQuaternion& rhs);
		constexpr TQuaternion& operator+=(const TQuaternion& rhs);

		constexpr T Length() const;
		constexpr T Length2() const;
		constexpr T Dot(const TQuaternion& rhs) const;
		constexpr void Normalize();
		constexpr void Conjugate();
		constexpr void Inverse();
		constexpr void Concatenate(const TQuaternion& rhs);

		constexpr TQuaternion Normalized() const;
		constexpr TQuaternion Conjugated() const;
		constexpr TQuaternion Inversed() const;
		constexpr TQuaternion Lerp(const TQuaternion& rhs, T t) const;
		constexpr TQuaternion Slerp(const TQuaternion& rhs, T t) const;

		constexpr void RotateX(T radians);
		constexpr void RotateY(T radians);
		constexpr void RotateZ(T radians);

		constexpr static TQuaternion Lerp(const TQuaternion& start, const TQuaternion& end, T t);
		constexpr static TQuaternion Slerp(const TQuaternion& start, const TQuaternion& end, T t);
		constexpr static TQuaternion FromAxisAngle(const Vector3<T>& axisAngle, T radians);
		constexpr static TQuaternion FromDegrees(const Vector3<T>& degrees);

		constexpr bool operator==(const TQuaternion& rhs) const;
		constexpr String ToString() const;
	};

	template<Numeric T>
	const TQuaternion<T> TQuaternion<T>::Identity = TQuaternion<T>();

	template<Numeric T>
	constexpr TQuaternion<T>::TQuaternion(T x, T y, T z, T w)
		: x(x)
		, y(y)
		, z(z)
		, w(w)
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
		const T newW = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		const T newX = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
		const T newY = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
		const T newZ = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
		return TQuaternion(newX, newY, newZ, newW);
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
	constexpr T TQuaternion<T>::Length() const
	{
		return std::sqrt(Length2());
	}

	template<Numeric T>
	constexpr T TQuaternion<T>::Length2() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::Normalize()
	{
		const T length = Length();
		if (length > 0.0f)
		{
			const T invLength = static_cast<T>(1) / length;
			x *= invLength;
			y *= invLength;
			z *= invLength;
			w *= invLength;
		}
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::Normalized() const
	{
		TQuaternion result = *this;
		result.Normalize();
		return result;
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::Conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::Conjugated() const
	{
		return TQuaternion(-x, -y, -z, w);
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::Inverse()
	{
		const T length2 = Length2();
		if (length2 > 0.0f)
		{
			const T invLength2 = static_cast<T>(1) / length2;
			x = -x * invLength2;
			y = -y * invLength2;
			z = -z * invLength2;
			w = w * invLength2;
		}
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::Concatenate(const TQuaternion& rhs)
	{
		const T newW = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		const T newX = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
		const T newY = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
		const T newZ = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
		x = newX;
		y = newY;
		z = newZ;
		w = newW;
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::Inversed() const
	{
		TQuaternion result = *this;
		result.Inverse();
		return result;
	}

	template<Numeric T>
	constexpr T TQuaternion<T>::Dot(const TQuaternion& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::Lerp(const TQuaternion& rhs, T t) const
	{
		const T t1 = static_cast<T>(1) - t;
		return TQuaternion(
			x * t1 + rhs.x * t,
			y * t1 + rhs.y * t,
			z * t1 + rhs.z * t,
			w * t1 + rhs.w * t);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::Slerp(const TQuaternion& rhs, T t) const
	{
		const T dot = Dot(rhs);
		const T epsilon = static_cast<T>(0.0001);
		if (dot > static_cast<T>(1) - epsilon)
		{
			return Lerp(rhs, t);
		}

		const T theta = std::acos(dot);
		const T theta1 = theta * (static_cast<T>(1) - t);
		const T theta2 = theta * t;
		const T sinTheta1 = std::sin(theta1);
		const T sinTheta2 = std::sin(theta2);
		const T sinTheta = std::sin(theta);

		const TQuaternion q1 = *this * sinTheta1 / sinTheta;
		const TQuaternion q2 = rhs * sinTheta2 / sinTheta;
		return q1 + q2;
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::RotateX(T radians)
	{
		const T halfAngle = radians * static_cast<T>(0.5);
		const T sinHalfAngle = std::sin(halfAngle);
		const T cosHalfAngle = std::cos(halfAngle);

		const T newX = x * cosHalfAngle + w * sinHalfAngle;
		const T newY = y * cosHalfAngle + z * sinHalfAngle;
		const T newZ = z * cosHalfAngle - y * sinHalfAngle;
		const T newW = w * cosHalfAngle - x * sinHalfAngle;

		x = newX;
		y = newY;
		z = newZ;
		w = newW;
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::RotateY(T radians)
	{
		const T halfAngle = radians * static_cast<T>(0.5);
		const T sinHalfAngle = std::sin(halfAngle);
		const T cosHalfAngle = std::cos(halfAngle);

		const T newX = x * cosHalfAngle - z * sinHalfAngle;
		const T newY = y * cosHalfAngle + w * sinHalfAngle;
		const T newZ = z * cosHalfAngle + x * sinHalfAngle;
		const T newW = w * cosHalfAngle - y * sinHalfAngle;

		x = newX;
		y = newY;
		z = newZ;
		w = newW;
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::RotateZ(T radians)
	{
		const T halfAngle = radians * static_cast<T>(0.5);
		const T sinHalfAngle = std::sin(halfAngle);
		const T cosHalfAngle = std::cos(halfAngle);

		const T newX = x * cosHalfAngle + y * sinHalfAngle;
		const T newY = y * cosHalfAngle - x * sinHalfAngle;
		const T newZ = z * cosHalfAngle + w * sinHalfAngle;
		const T newW = w * cosHalfAngle - z * sinHalfAngle;

		x = newX;
		y = newY;
		z = newZ;
		w = newW;
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::Lerp(const TQuaternion& start, const TQuaternion& end, T t)
	{
		const T t1 = static_cast<T>(1) - t;
		return TQuaternion(
			start.x * t1 + end.x * t,
			start.y * t1 + end.y * t,
			start.z * t1 + end.z * t,
			start.w * t1 + end.w * t);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::Slerp(const TQuaternion& start, const TQuaternion& end, T t)
	{
		const T dot = start.Dot(end);
		const T epsilon = static_cast<T>(0.0001);
		if (dot > static_cast<T>(1) - epsilon)
		{
			return Lerp(start, end, t);
		}

		const T theta = std::acos(dot);
		const T theta1 = theta * (static_cast<T>(1) - t);
		const T theta2 = theta * t;
		const T sinTheta1 = std::sin(theta1);
		const T sinTheta2 = std::sin(theta2);
		const T sinTheta = std::sin(theta);

		const TQuaternion q1 = start * sinTheta1 / sinTheta;
		const TQuaternion q2 = end * sinTheta2 / sinTheta;
		return q1 + q2;
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::FromAxisAngle(const Vector3<T>& axisAngle, T radians)
	{
		const T halfAngle = radians * static_cast<T>(0.5);
		const T sinHalfAngle = std::sin(halfAngle);
		const T cosHalfAngle = std::cos(halfAngle);

		const T x = axisAngle.x * sinHalfAngle;
		const T y = axisAngle.y * sinHalfAngle;
		const T z = axisAngle.z * sinHalfAngle;
		const T w = cosHalfAngle;

		return TQuaternion(x, y, z, w);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::FromDegrees(const Vector3<T>& degrees)
	{
		const TQuaternion<T> x = FromAxisAngle(Vector3<T>::Right(), ToRadians(degrees.x));
		const TQuaternion<T> y = FromAxisAngle(Vector3<T>::Up(), ToRadians(degrees.y));
		const TQuaternion<T> z = FromAxisAngle(Vector3<T>::Forward(), ToRadians(degrees.z));

		return x * y * z;
	}

	template<Numeric T>
	constexpr bool TQuaternion<T>::operator==(const TQuaternion& rhs) const
	{
		return AreValuesClose(x, rhs.x, static_cast<T>(0.001)) &&
			   AreValuesClose(y, rhs.y, static_cast<T>(0.001)) &&
			   AreValuesClose(z, rhs.z, static_cast<T>(0.001)) &&
			   AreValuesClose(w, rhs.w, static_cast<T>(0.001));
	}

	template<Numeric T>
	constexpr String TQuaternion<T>::ToString() const
	{
		jpt::String result;
		result += "x: ";
		result += jpt::ToString(x);
		result += ", y:";
		result += jpt::ToString(y);
		result += ", z:";
		result += jpt::ToString(z);
		result += ", w:";
		result += jpt::ToString(w);
		return result;
	}
}

export using Quatf = jpt::TQuaternion<float>;
export using Quatd = jpt::TQuaternion<float>;