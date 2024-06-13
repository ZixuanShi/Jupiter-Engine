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

		constexpr T Length() const;
		constexpr T Length2() const;
		constexpr void Normalize();
		constexpr TQuaternion Normalized() const;
		constexpr void Conjugate();
		constexpr TQuaternion Conjugated() const;
		constexpr void Inverse();
		constexpr TQuaternion Inversed() const;

		constexpr T Dot(const TQuaternion& rhs) const;

		constexpr TQuaternion Lerp(const TQuaternion& rhs, T t) const;
		constexpr TQuaternion Slerp(const TQuaternion& rhs, T t) const;

		constexpr void Rotate(const Vector3<T>& axis, T radians);
		constexpr void Rotate(const Vector3<T>& axis, T radians, const Vector3<T>& pivot);
		constexpr void RotateX(T radians);
		constexpr void RotateY(T radians);
		constexpr void RotateZ(T radians);
		constexpr void LookAt(const Vector3<T>& target, const Vector3<T>& up);

		constexpr static TQuaternion FromEulerAngles(const Vector3<T>& eulerAngles);
		constexpr static TQuaternion FromAxisAngle(const Vector3<T>& axis, T radians);
		constexpr static TQuaternion Lerp(const TQuaternion& start, const TQuaternion& end, T t);
		constexpr static TQuaternion Slerp(const TQuaternion& start, const TQuaternion& end, T t);
		constexpr static TQuaternion Rotation(const TQuaternion& quaternion, const Vector3<T>& axis, T radians);
		constexpr static TQuaternion RotationX(const TQuaternion& quaternion, T radians);
		constexpr static TQuaternion RotationY(const TQuaternion& quaternion, T radians);
		constexpr static TQuaternion RotationZ(const TQuaternion& quaternion, T radians);
		constexpr static TQuaternion Rotation(const TQuaternion& quaternion, const Vector3<T>& axis, T radians, const Vector3<T>& pivot);

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
		TQuaternion result = *this;
		result.Conjugate();
		return result;
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
	constexpr void TQuaternion<T>::Rotate(const Vector3<T>& axis, T radians)
	{
		const T halfAngle = radians * static_cast<T>(0.5);
		const T sinHalfAngle = std::sin(halfAngle);
		const T cosHalfAngle = std::cos(halfAngle);

		const T newX = axis.x * sinHalfAngle;
		const T newY = axis.y * sinHalfAngle;
		const T newZ = axis.z * sinHalfAngle;
		const T newW = cosHalfAngle;

		const TQuaternion rotation(newX, newY, newZ, newW);
		*this = rotation * *this;
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::Rotate(const Vector3<T>& axis, T radians, const Vector3<T>& pivot)
	{
		const Vector3<T> translation = pivot - Vector3<T>(0, 0, 0);
		const Vector3<T> translationRotation = translation * -1;
		const Vector3<T> rotationAxis = axis.Normalized();
		const TQuaternion rotation(rotationAxis, radians);
		const TQuaternion translationQuaternion(translationRotation);
		const TQuaternion translationQuaternionConjugate = translationQuaternion.Conjugated();

		*this = translationQuaternion * rotation * translationQuaternionConjugate * *this;
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::RotateX(T radians)
	{
		Vector3<T> axis(1, 0, 0);
		Rotate(axis, radians);
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::RotateY(T radians)
	{
		Vector3<T> axis(0, 1, 0);
		Rotate(axis, radians);
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::RotateZ(T radians)
	{
		Vector3<T> axis(0, 0, 1);
		Rotate(axis, radians);
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::LookAt(const Vector3<T>& target, const Vector3<T>& up)
	{
		const Vector3<T> forward = (target - Vector3<T>(0, 0, 0)).Normalized();
		const Vector3<T> right = up.Cross(forward).Normalized();
		const Vector3<T> upVector = forward.Cross(right).Normalized();

		const T trace = right.x + upVector.y + forward.z;
		if (trace > static_cast<T>(0))
		{
			const T s = std::sqrt(trace + static_cast<T>(1)) * static_cast<T>(2);
			const T invS = static_cast<T>(1) / s;

			x = (upVector.z - forward.y) * invS;
			y = (forward.x - right.z) * invS;
			z = (right.y - upVector.x) * invS;
			w = static_cast<T>(0.25) * s;
		}
		else if (right.x > upVector.y && right.x > forward.z)
		{
			const T s = std::sqrt(static_cast<T>(1) + right.x - upVector.y - forward.z) * static_cast<T>(2);
			const T invS = static_cast<T>(1) / s;

			x = static_cast<T>(0.25) * s;
			y = (right.y + upVector.x) * invS;
			z = (forward.x + right.z) * invS;
			w = (upVector.z - forward.y) * invS;
		}
		else if (upVector.y > forward.z)
		{
			const T s = std::sqrt(static_cast<T>(1) + upVector.y - right.x - forward.z) * static_cast<T>(2);
			const T invS = static_cast<T>(1) / s;

			x = (right.y + upVector.x) * invS;
			y = static_cast<T>(0.25) * s;
			z = (upVector.z + forward.y) * invS;
			w = (forward.x - right.z) * invS;
		}
		else
		{
			const T s = std::sqrt(static_cast<T>(1) + forward.z - right.x - upVector.y) * static_cast<T>(2);
			const T invS = static_cast<T>(1) / s;

			x = (forward.x + right.z) * invS;
		}
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::FromEulerAngles(const Vector3<T>& eulerAngles)
	{
		const T halfX = eulerAngles.x * static_cast<T>(0.5);
		const T halfY = eulerAngles.y * static_cast<T>(0.5);
		const T halfZ = eulerAngles.z * static_cast<T>(0.5);

		const T sinX = std::sin(halfX);
		const T cosX = std::cos(halfX);
		const T sinY = std::sin(halfY);
		const T cosY = std::cos(halfY);
		const T sinZ = std::sin(halfZ);
		const T cosZ = std::cos(halfZ);

		const T newX = sinX * cosY * cosZ + cosX * sinY * sinZ;
		const T newY = cosX * sinY * cosZ - sinX * cosY * sinZ;
		const T newZ = cosX * cosY * sinZ - sinX * sinY * cosZ;
		const T newW = cosX * cosY * cosZ + sinX * sinY * sinZ;

		return TQuaternion(newX, newY, newZ, newW);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::FromAxisAngle(const Vector3<T>& axis, T radians)
	{
		const T halfAngle = radians * static_cast<T>(0.5);
		const T sinHalfAngle = std::sin(halfAngle);
		const T cosHalfAngle = std::cos(halfAngle);

		const T newX = axis.x * sinHalfAngle;
		const T newY = axis.y * sinHalfAngle;
		const T newZ = axis.z * sinHalfAngle;
		const T newW = cosHalfAngle;

		return TQuaternion(newX, newY, newZ, newW);
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
	constexpr TQuaternion<T> TQuaternion<T>::Rotation(const TQuaternion& quaternion, const Vector3<T>& axis, T radians)
	{
		const T halfAngle = radians * static_cast<T>(0.5);
		const T sinHalfAngle = std::sin(halfAngle);
		const T cosHalfAngle = std::cos(halfAngle);

		const T newX = axis.x * sinHalfAngle;
		const T newY = axis.y * sinHalfAngle;
		const T newZ = axis.z * sinHalfAngle;
		const T newW = cosHalfAngle;

		const TQuaternion rotation(newX, newY, newZ, newW);
		return rotation * quaternion;
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::RotationX(const TQuaternion& quaternion, T radians)
	{
		Vector3<T> axis(1, 0, 0);
		return Rotation(quaternion, axis, radians);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::RotationY(const TQuaternion& quaternion, T radians)
	{
		Vector3<T> axis(0, 1, 0);
		return Rotation(quaternion, axis, radians);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::RotationZ(const TQuaternion& quaternion, T radians)
	{
		Vector3<T> axis(0, 0, 1);
		return Rotation(quaternion, axis, radians);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::Rotation(const TQuaternion& quaternion, const Vector3<T>& axis, T radians, const Vector3<T>& pivot)
	{
		const Vector3<T> translation = pivot - Vector3<T>(0, 0, 0);
		const Vector3<T> translationRotation = translation * -1;
		const Vector3<T> rotationAxis = axis.Normalized();
		const TQuaternion rotation(rotationAxis, radians);
		const TQuaternion translationQuaternion(translationRotation);
		const TQuaternion translationQuaternionConjugate = translationQuaternion.Conjugated();

		return translationQuaternion * rotation * translationQuaternionConjugate * quaternion;
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