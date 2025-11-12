// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <cmath>

export module jpt.Quaternion;

import jpt.Concepts;
import jpt.Constants;
import jpt.Math;
import jpt.String;
import jpt.TypeDefs;
import jpt.Vector3;

export namespace jpt
{
    template<Numeric T>
    struct TQuaternion
    {
    public:
        T x = static_cast<T>(0);    // Pitch
        T y = static_cast<T>(0);    // Yaw
        T z = static_cast<T>(0);    // Roll
        T w = static_cast<T>(1);    // Scalar

    public:
        using NumericType = T;

        static consteval TQuaternion Identity() { return TQuaternion(); }

    public:
        constexpr TQuaternion() = default;
        constexpr TQuaternion(T _x, T _y, T _z, T _w);

    public:
        [[nodiscard]] constexpr TQuaternion operator*(const TQuaternion& rhs) const noexcept;
        constexpr TQuaternion& operator*=(const TQuaternion& rhs) noexcept;

    public:
        // Properties
        [[nodiscard]] constexpr T Length() const;
        [[nodiscard]] constexpr T Length2() const;
        [[nodiscard]] constexpr T Dot(const TQuaternion& rhs) const;

        // Operations
        constexpr void Normalize() noexcept;
        [[nodiscard]] constexpr TQuaternion Normalized() const noexcept;
        [[nodiscard]] constexpr TQuaternion Conjugate()  const noexcept;
        [[nodiscard]] constexpr TQuaternion Inverse()    const noexcept;

        // Interpolation
        [[nodiscard]] constexpr static TQuaternion Lerp(const TQuaternion& start, const TQuaternion& end, T t) noexcept;
        [[nodiscard]] constexpr static TQuaternion Slerp(const TQuaternion& start, const TQuaternion& end, T t) noexcept;

        // Rotation & Orientation
        [[nodiscard]] constexpr static TQuaternion FromAxisAngle(const Vector3<T>& axisAngle, T radians) noexcept;
        [[nodiscard]] constexpr static TQuaternion FromEulerAngles(const Vector3<T>& eulerAngles) noexcept;
        [[nodiscard]] constexpr Vector3<T> CalcEulerAngles() const;

        // Directions
        [[nodiscard]] constexpr Vector3<T> Forward() const noexcept;
        [[nodiscard]] constexpr Vector3<T> Right() const noexcept;
        [[nodiscard]] constexpr Vector3<T> Up() const noexcept;
    };

    // ------------------------------------------------------------------------------------------------
    // Non-Member Functions
    // ------------------------------------------------------------------------------------------------
    template<Numeric T>
    [[nodiscard]] constexpr bool operator==(const TQuaternion<T>& lhs, const TQuaternion<T>& rhs)
    {
        return AreValuesClose(lhs.x, rhs.x) &&
               AreValuesClose(lhs.y, rhs.y) &&
               AreValuesClose(lhs.z, rhs.z) &&
               AreValuesClose(lhs.w, rhs.w);
    }

    template<Numeric T>
    [[nodiscard]] constexpr TQuaternion<T> operator+(const TQuaternion<T>& lhs, const TQuaternion<T>& rhs) noexcept
    {
        return TQuaternion(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
    }

    template<Numeric T>
    [[nodiscard]] constexpr TQuaternion<T> operator-(const TQuaternion<T>& lhs, const TQuaternion<T>& rhs) noexcept
    {
        return TQuaternion(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
    }

    template<Numeric T>
    [[nodiscard]] constexpr TQuaternion<T> operator/(const TQuaternion<T>& lhs, const TQuaternion<T>& rhs) noexcept
    {
        return TQuaternion(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
    }

    template<Numeric T>
    [[nodiscard]] constexpr TQuaternion<T> operator/(const TQuaternion<T>& q, T scalar) noexcept
    {
        return TQuaternion(q.x / scalar, q.y / scalar, q.z / scalar, q.w / scalar);
    }

    template<Numeric T>
    [[nodiscard]] constexpr TQuaternion<T> operator*(const TQuaternion<T>& q, T scalar) noexcept
    {
        return TQuaternion(q.x * scalar, q.y * scalar, q.z * scalar, q.w * scalar);
    }

    template<Numeric T>
    [[nodiscard]] constexpr TQuaternion<T> operator/(T scalar, const TQuaternion<T>& q) noexcept
    {
        return TQuaternion(q.x / scalar, q.y / scalar, q.z / scalar, q.w / scalar);
    }

    template<Numeric T>
    [[nodiscard]] constexpr TQuaternion<T> operator*(T scalar, const TQuaternion<T>& q) noexcept
    {
        return TQuaternion(q.x * scalar, q.y * scalar, q.z * scalar, q.w * scalar);
    }

    template<Numeric T>
    [[nodiscard]] constexpr String ToString(const TQuaternion<T>& quaternion)
    {
        return String::Format<64>("x: %.3f, y: %.3f, z: %.3f, w: %.3f", quaternion.x, quaternion.y, quaternion.z, quaternion.w);
    }

    // ------------------------------------------------------------------------------------------------
    // Member Functions
    // ------------------------------------------------------------------------------------------------
    template<Numeric T>
    constexpr TQuaternion<T>::TQuaternion(T _x, T _y, T _z, T _w)
        : x(_x)
        , y(_y)
        , z(_z)
        , w(_w)
    {
    }

    template<Numeric T>
    constexpr TQuaternion<T> TQuaternion<T>::operator*(const TQuaternion& rhs) const noexcept
    {
        const T newW = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
        const T newX = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
        const T newY = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
        const T newZ = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
        return TQuaternion(newX, newY, newZ, newW);
    }

    template<Numeric T>
    constexpr TQuaternion<T>& TQuaternion<T>::operator*=(const TQuaternion& rhs) noexcept
    {
        return *this = *this * rhs;
    }

    template<Numeric T>
    constexpr T TQuaternion<T>::Length() const
    {
        return Sqrt(Length2());
    }

    template<Numeric T>
    constexpr T TQuaternion<T>::Length2() const
    {
        return x * x + y * y + z * z + w * w;
    }

    template<Numeric T>
    constexpr void TQuaternion<T>::Normalize() noexcept
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
    constexpr TQuaternion<T> TQuaternion<T>::Normalized() const noexcept
    {
        TQuaternion result = *this;
        result.Normalize();
        return result;
    }

    template<Numeric T>
    constexpr TQuaternion<T> TQuaternion<T>::Conjugate() const noexcept
    {
        return TQuaternion(-x, -y, -z, w);
    }

    template<Numeric T>
    constexpr TQuaternion<T> TQuaternion<T>::Inverse() const noexcept
    {
        const T length2 = Length2();
        if (length2 > 0.0f)
        {
            const T invLength2 = static_cast<T>(1) / length2;
            return Conjugate() * invLength2;
        }
        return *this;
    }

    template<Numeric T>
    constexpr T TQuaternion<T>::Dot(const TQuaternion& rhs) const
    {
        return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
    }

    template<Numeric T>
    constexpr TQuaternion<T> TQuaternion<T>::Lerp(const TQuaternion& start, const TQuaternion& end, T t) noexcept
    {
        const T t1 = static_cast<T>(1) - t;
        return TQuaternion(start.x * t1 + end.x * t,
                           start.y * t1 + end.y * t,
                           start.z * t1 + end.z * t,
                           start.w * t1 + end.w * t);
    }

    template<Numeric T>
    constexpr TQuaternion<T> TQuaternion<T>::Slerp(const TQuaternion& start, const TQuaternion& end, T t) noexcept
    {
        T dot = start.Dot(end);

        // Take the shorter path by negating end if dot is negative
        TQuaternion adjustedEnd = end;
        if (dot < static_cast<T>(0))
        {
            adjustedEnd = end * static_cast<T>(-1);
            dot = -dot;
        }

        // If quaternions are very close, use linear interpolation
        if (dot > static_cast<T>(1) - kEpsilon<T>)
        {
            return Lerp(start, adjustedEnd, t).Normalized();
        }

        const T theta = Acos(dot);
        const T sinTheta = Sin(theta);
        const T theta1 = theta * (static_cast<T>(1) - t);
        const T theta2 = theta * t;
        const T sinTheta1 = Sin(theta1);
        const T sinTheta2 = Sin(theta2);

        const TQuaternion q1 = (start * sinTheta1) / sinTheta;
        const TQuaternion q2 = (adjustedEnd * sinTheta2) / sinTheta;
        return q1 + q2;
    }

    template<Numeric T>
    constexpr TQuaternion<T> TQuaternion<T>::FromAxisAngle(const Vector3<T>& axisAngle, T radians) noexcept
    {
        JPT_ASSERT(axisAngle.Normalized() == axisAngle, "Axis Angle must be normalized to be converted to Quaternion");

        const T halfAngle = radians * static_cast<T>(0.5);
        const T sinHalfAngle = Sin(halfAngle);
        const T cosHalfAngle = Cos(halfAngle);

        const T _x = axisAngle.x * sinHalfAngle;
        const T _y = axisAngle.y * sinHalfAngle;
        const T _z = axisAngle.z * sinHalfAngle;
        const T _w = cosHalfAngle;

        return TQuaternion(_x, _y, _z, _w);
    }

    template<Numeric T>
    constexpr TQuaternion<T> TQuaternion<T>::FromEulerAngles(const Vector3<T>& eulerAngles) noexcept
    {
        const T halfX = eulerAngles.x * static_cast<T>(0.5); // pitch
        const T halfY = eulerAngles.y * static_cast<T>(0.5); // yaw
        const T halfZ = eulerAngles.z * static_cast<T>(0.5); // roll

        const T cx = Cos(halfX);
        const T sx = Sin(halfX);
        const T cy = Cos(halfY);
        const T sy = Sin(halfY);
        const T cz = Cos(halfZ);
        const T sz = Sin(halfZ);

        // Compute quaternion components for right-handed system. XYZ order
        const T _w = cx * cy * cz - sx * sy * sz;
        const T _x = sx * cy * cz + cx * sy * sz;
        const T _y = cx * sy * cz - sx * cy * sz;
        const T _z = cx * cy * sz + sx * sy * cz;

        return TQuaternion(_x, _y, _z, _w);
    }

    template<Numeric T>
    constexpr Vector3<T> TQuaternion<T>::CalcEulerAngles() const
    {
        // Convert quaternion to Euler angles (pitch, yaw, roll)
        // Right-handed system, XYZ order (matching Matrix44::CalcEulerAngles())

        const TQuaternion<T> q = Normalized();

        // Convert to rotation matrix elements (matching Matrix44 column-major layout)
        // rotation[i][j] means column i, row j
        const T r00 = static_cast<T>(1) - static_cast<T>(2) * (q.y * q.y + q.z * q.z);
        const T r10 = static_cast<T>(2) * (q.x * q.y + q.w * q.z);
        const T r20 = static_cast<T>(2) * (q.x * q.z - q.w * q.y);
        const T r21 = static_cast<T>(2) * (q.y * q.z + q.w * q.x);
        const T r22 = static_cast<T>(1) - static_cast<T>(2) * (q.x * q.x + q.y * q.y);

        // Same formula as Matrix44::GetRotation()
        const T sy = Sqrt(r00 * r00 + r10 * r10);

        if (sy > kEpsilon<T>)
        {
            const T pitch = -Atan2(r21, r22);
            const T yaw = -Atan2(-r20, sy);
            const T roll = -Atan2(r10, r00);
            return Vector3<T>(pitch, yaw, roll);
        }
        else // Gimbal lock
        {
            JPT_ASSERT(false, "Gimbal lock detected");
            return Vector3<T>(0);
        }
    }

    template<Numeric T>
    constexpr Vector3<T> TQuaternion<T>::Forward() const noexcept
    {
        const T _x = static_cast<T>(2) * (x * z - w * y);
        const T _y = static_cast<T>(2) * (y * z + w * x);
        const T _z = static_cast<T>(1) - static_cast<T>(2) * (x * x + y * y);
        return Vector3<T>(_x, _y, _z);
    }

    template<Numeric T>
    constexpr Vector3<T> TQuaternion<T>::Right() const noexcept
    {
        const T _x = static_cast<T>(1) - static_cast<T>(2) * (y * y + z * z);
        const T _y = static_cast<T>(2) * (x * y - w * z);
        const T _z = static_cast<T>(2) * (x * z + w * y);
        return Vector3<T>(_x, _y, _z);
    }

    template<Numeric T>
    constexpr Vector3<T> TQuaternion<T>::Up() const noexcept
    {
        const T _x = static_cast<T>(2) * (x * y + w * z);
        const T _y = static_cast<T>(1) - static_cast<T>(2) * (x * x + z * z);
        const T _z = static_cast<T>(2) * (y * z - w * x);
        return Vector3<T>(_x, _y, _z);
    }
}

export using Quaternion  = jpt::TQuaternion<Precision>;
export using Quaternionf = jpt::TQuaternion<float32>;
export using Quaterniond = jpt::TQuaternion<float64>;