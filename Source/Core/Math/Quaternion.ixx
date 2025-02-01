// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <cmath>

export module jpt.Quaternion;

import jpt.Concepts;
import jpt.Constants;
import jpt.Math;
import jpt.Math.Settings;
import jpt.String;
import jpt.ToString;
import jpt.TypeDefs;
import jpt.Vector3;

export namespace jpt
{
	template<Numeric T>
	struct TQuaternion
	{
	public:
		T x = static_cast<T>(0);
		T y = static_cast<T>(0);
		T z = static_cast<T>(0);
		T w = static_cast<T>(1);

	public:
		using NumericType = T;

		static consteval TQuaternion Identity() { return TQuaternion(); }

	public:
		constexpr TQuaternion() = default;
		constexpr TQuaternion(T x, T y, T z, T w);

	public:
		constexpr TQuaternion operator+(const TQuaternion& rhs) const;
		constexpr TQuaternion operator-(const TQuaternion& rhs) const;
		constexpr TQuaternion operator*(const TQuaternion& rhs) const;
		constexpr TQuaternion operator/(const TQuaternion& rhs) const;

		constexpr TQuaternion operator/(T scalar) const;
		constexpr TQuaternion operator*(T scalar) const;

		constexpr TQuaternion& operator*=(const TQuaternion& rhs);
		constexpr TQuaternion& operator+=(const TQuaternion& rhs);

	public:
		constexpr T Length() const;
		constexpr T Length2() const;
		constexpr T Dot(const TQuaternion& rhs) const;
		constexpr void Normalize();
		constexpr void Conjugate();
		constexpr void Inverse();

		constexpr TQuaternion Normalized() const;
		constexpr TQuaternion Conjugated() const;
		constexpr TQuaternion Inversed() const;

		constexpr static TQuaternion Lerp(const TQuaternion& start, const TQuaternion& end, T t);
		constexpr static TQuaternion Slerp(const TQuaternion& start, const TQuaternion& end, T t);
		constexpr TQuaternion Lerp(const TQuaternion& rhs, T t) const;
		constexpr TQuaternion Slerp(const TQuaternion& rhs, T t) const;

		constexpr static TQuaternion Rotation(const Vector3<T>& axisAngle, T radians);
		constexpr static TQuaternion Rotation(const Vector3<T>& radians);
		constexpr static TQuaternion Rotation(T pitch, T yaw, T roll);
		constexpr void RotateX(T radians);
		constexpr void RotateY(T radians);
		constexpr void RotateZ(T radians);
		constexpr Vector3<T> GetRotation() const;	// Returns pitch, yaw, roll in radians

		constexpr String ToString() const;
	};

	template<Numeric T>
	constexpr bool operator==(const TQuaternion<T>& lhs, const TQuaternion<T>& rhs)
	{
		return AreValuesClose(lhs.x, rhs.x, static_cast<T>(0.001)) &&
			   AreValuesClose(lhs.y, rhs.y, static_cast<T>(0.001)) &&
			   AreValuesClose(lhs.z, rhs.z, static_cast<T>(0.001)) &&
			   AreValuesClose(lhs.w, rhs.w, static_cast<T>(0.001));
	}

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
		const T sinTheta1 = Sin(theta1);
		const T sinTheta2 = Sin(theta2);
		const T sinTheta = Sin(theta);

		const TQuaternion q1 = *this * sinTheta1 / sinTheta;
		const TQuaternion q2 = rhs * sinTheta2 / sinTheta;
		return q1 + q2;
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::RotateX(T radians)
	{
		const TQuaternion<T> rotation = Rotation(Vector3<T>::Right(), radians);
		*this *= rotation;
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::RotateY(T radians)
	{
		const TQuaternion<T> rotation = Rotation(Vector3<T>::Up(), radians);
		*this *= rotation;
	}

	template<Numeric T>
	constexpr void TQuaternion<T>::RotateZ(T radians)
	{
		const TQuaternion<T> rotation = Rotation(Vector3<T>::Forward(), radians);
		*this *= rotation;
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
		const T sinTheta1 = Sin(theta1);
		const T sinTheta2 = Sin(theta2);
		const T sinTheta = Sin(theta);

		const TQuaternion q1 = start * sinTheta1 / sinTheta;
		const TQuaternion q2 = end * sinTheta2 / sinTheta;
		return q1 + q2;
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::Rotation(const Vector3<T>& axisAngle, T radians)
	{
		JPT_ASSERT(axisAngle.Normalized() == axisAngle, "Axis Angle must be normalized to be converted to Quaternion");

		const T halfAngle = radians * static_cast<T>(0.5);
		const T sinHalfAngle = Sin(halfAngle);
		const T cosHalfAngle = Cos(halfAngle);

		const T x = axisAngle.x * sinHalfAngle;
		const T y = axisAngle.y * sinHalfAngle;
		const T z = axisAngle.z * sinHalfAngle;
		const T w = cosHalfAngle;

		return TQuaternion(x, y, z, w);
	}

	template<Numeric T>
	constexpr Vector3<T> TQuaternion<T>::GetRotation() const
	{
		Vector3<T> euler;

		// Convert quaternion to rotation matrix elements for easier conversion
		const T xx = x * x;
		const T xy = x * y;
		const T xz = x * z;
		const T xw = x * w;
		const T yy = y * y;
		const T yz = y * z;
		const T yw = y * w;
		const T zz = z * z;
		const T zw = z * w;

		const T m11 = 1 - 2 * (yy + zz);
		const T m12 = 2 * (xy - zw);
		const T m13 = 2 * (xz + yw);
		const T m21 = 2 * (xy + zw);
		const T m22 = 1 - 2 * (xx + zz);
		const T m23 = 2 * (yz - xw);
		const T m31 = 2 * (xz - yw);
		const T m32 = 2 * (yz + xw);
		const T m33 = 1 - 2 * (xx + yy);

		switch (MathSettings::RotationOrder)
		{
			case RotationOrder::XYZ:
			{
				euler.y = std::asin(Clamp(m13, static_cast<T>(-1), static_cast<T>(1)));

				if (std::abs(m13) < static_cast<T>(0.9999999))
				{
					euler.x = std::atan2(-m23, m33);
					euler.z = std::atan2(-m12, m11);
				}
				else
				{
					euler.x = std::atan2(m32, m22);
					euler.z = 0;
				}
				break;
			}
			case RotationOrder::XZY:
			{
				euler.z = std::asin(Clamp(-m12, static_cast<T>(-1), static_cast<T>(1)));

				if (std::abs(m12) < static_cast<T>(0.9999999))
				{
					euler.x = std::atan2(m32, m22);
					euler.y = std::atan2(m13, m11);
				}
				else
				{
					euler.x = std::atan2(-m23, m33);
					euler.y = 0;
				}
				break;
			}
			[[likely]] case RotationOrder::YXZ:
			{
				euler.x = std::asin(Clamp(-m23, static_cast<T>(-1), static_cast<T>(1)));  // Pitch

				if (std::abs(m23) < static_cast<T>(0.9999999))
				{
					euler.y = std::atan2(m13, m33);     // Yaw
					euler.z = std::atan2(m21, m22);     // Roll
				}
				else
				{
					// Gimbal lock - arbitrary choice for remaining angle
					euler.y = std::atan2(-m31, m11);    // Yaw
					euler.z = 0;                        // Roll
				}
				break;
			}
			case RotationOrder::YZX:
			{
				euler.z = std::asin(Clamp(m21, static_cast<T>(-1), static_cast<T>(1)));

				if (std::abs(m21) < static_cast<T>(0.9999999))
				{
					euler.y = std::atan2(-m23, m22);
					euler.x = std::atan2(-m31, m11);
				}
				else
				{
					euler.y = std::atan2(m13, m33);
					euler.x = 0;
				}
				break;
			}
			case RotationOrder::ZXY:
			{
				euler.x = std::asin(Clamp(m32, static_cast<T>(-1), static_cast<T>(1)));

				if (std::abs(m32) < static_cast<T>(0.9999999))
				{
					euler.z = std::atan2(-m12, m22);
					euler.y = std::atan2(-m31, m33);
				}
				else
				{
					euler.z = std::atan2(m21, m11);
					euler.y = 0;
				}
				break;
			}
			case RotationOrder::ZYX:
			{
				euler.y = std::asin(Clamp(-m31, static_cast<T>(-1), static_cast<T>(1)));

				if (std::abs(m31) < static_cast<T>(0.9999999))
				{
					euler.z = std::atan2(m21, m11);
					euler.x = std::atan2(m32, m33);
				}
				else
				{
					euler.z = std::atan2(-m12, m22);
					euler.x = 0;
				}
				break;
			}
			default:
			{
				JPT_ASSERT(false, "Invalid Rotation Order");
				return Vector3<T>();
			}
		}

		return euler;
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::Rotation(const Vector3<T>& radians)
	{
		return Rotation(radians.x, radians.y, radians.z);
	}

	template<Numeric T>
	constexpr TQuaternion<T> TQuaternion<T>::Rotation(T pitch, T yaw, T roll)
	{
		// Calculate Half angles
		const T cx = Cos(pitch * static_cast<T>(0.5));
		const T cy = Cos(yaw   * static_cast<T>(0.5));
		const T cz = Cos(roll  * static_cast<T>(0.5));
		const T sx = Sin(pitch * static_cast<T>(0.5));
		const T sy = Sin(yaw   * static_cast<T>(0.5));
		const T sz = Sin(roll  * static_cast<T>(0.5));

		// Apply formula
		T qw = 0;
		T qx = 0;
		T qy = 0;
		T qz = 0;

		switch (MathSettings::RotationOrder)
		{
			case RotationOrder::XYZ:
			{
				qw = cx * cy * cz - sx * sy * sz;
				qx = sx * cy * cz + cx * sy * sz;
				qy = cx * sy * cz - sx * cy * sz;
				qz = cx * cy * sz + sx * sy * cz;
				return TQuaternion<T>(qx, qy, qz, qw);
			}
			case RotationOrder::XZY:
			{
				qw = cx * cz * cy + sx * sy * sz;
				qx = sx * cz * cy - cx * sy * sz;
				qy = cx * sy * cz + sx * cy * sz;
				qz = cx * cy * sz - sx * sy * cz;
				return TQuaternion<T>(qx, qy, qz, qw);
			}
			[[likely]] case RotationOrder::YXZ:
			{
				qw = cy * cx * cz + sy * sx * sz;
				qx = cy * sx * cz + sy * cx * sz;
				qy = sy * cx * cz - cy * sx * sz;
				qz = cy * cx * sz - sy * sx * cz;
				return TQuaternion<T>(qx, qy, qz, qw);
			}
			case RotationOrder::YZX:
			{
				qw = cy * cz * cx - sy * sx * sz;
				qx = cy * sz * cx + sy * sx * cz;
				qy = sy * cz * cx + cy * sx * sz;
				qz = cy * sx * cz - sy * sz * cx;
				return TQuaternion<T>(qx, qy, qz, qw);
			}
			case RotationOrder::ZXY:
			{
				qw = cz * cx * cy - sz * sx * sy;
				qx = cz * sx * cy + sz * cx * sy;
				qy = cz * cx * sy + sz * sx * cy;
				qz = sz * cx * cy - cz * sx * sy;
				return TQuaternion<T>(qx, qy, qz, qw);
			}
			case RotationOrder::ZYX:
			{
				qw = cz * cy * cx + sz * sy * sx;
				qx = cz * cy * sx - sz * sy * cx;
				qy = cz * sy * cx + sz * cy * sx;
				qz = sz * cy * cx - cz * sy * sx;
				return TQuaternion<T>(qx, qy, qz, qw);
			}
			default:
			{
				JPT_ASSERT(false, "Invalid Rotation Order");
				return TQuaternion<T>();
			}
		}
	}

	template<Numeric T>
	constexpr String TQuaternion<T>::ToString() const
	{
		return String::Format<64>("x: %.3f, y: %.3f, z: %.3f, w: %.3f", x, y, z, w);
	}
}

export using Quaternion  = jpt::TQuaternion<Precision>;
export using Quaternionf = jpt::TQuaternion<float32>;
export using Quaterniond = jpt::TQuaternion<float64>;