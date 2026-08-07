// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Quaternion;

import jpt.Assert;
import jpt.Concepts;
import jpt.Constants;
import jpt.Math;
import jpt.Matrix44;
import jpt.TypeDefs;
import jpt.Vector3;
import jpt.Vector4;
import std;

export namespace jpt
{
    /** Right-handed, matching Matrix44: a positive angle about an axis carries the next basis
        vector in the cyclic order X -> Y -> Z(backward) -> X. */
    template<Floating T>
    struct Quaternion
    {
    public:
        [[nodiscard]] static consteval Quaternion Identity() noexcept { return Quaternion(); }

    public:
        T x = static_cast<T>(0);
        T y = static_cast<T>(0);
        T z = static_cast<T>(0);
        T w = static_cast<T>(1);    /**< Scalar */

    public:
        constexpr Quaternion() noexcept = default;
        constexpr Quaternion(T inX, T inY, T inZ, T inW) noexcept;

    public:
        /** Hamilton product. Not commutative: rhs is applied to a vector first. */
        [[nodiscard]] constexpr Quaternion operator*(const Quaternion& rhs) const noexcept;
        constexpr Quaternion& operator*=(const Quaternion& rhs) noexcept;

        [[nodiscard]] constexpr bool operator==(const Quaternion& other) const noexcept = default;

    public:
        [[nodiscard]] constexpr T Dot(const Quaternion& other) const noexcept;
        [[nodiscard]] constexpr T Length2() const noexcept;
        [[nodiscard]] T Length() const noexcept;

        /** Guards against kEpsilon rather than exact zero: a denormal length divides to inf. */
        void Normalize() noexcept;
        [[nodiscard]] Quaternion Normalized() const noexcept;

        [[nodiscard]] constexpr Quaternion Conjugate() const noexcept;
        [[nodiscard]] constexpr Quaternion Inverse() const noexcept;

    public:
        [[nodiscard]] static constexpr Quaternion Lerp(const Quaternion& start, const Quaternion& end, T t) noexcept;
        [[nodiscard]] static Quaternion Slerp(const Quaternion& start, const Quaternion& end, T t) noexcept;

        [[nodiscard]] static Quaternion FromAxisAngle(const Vector3<T>& axis, T radians) noexcept;

        /** X * Y * Z, Z applied first -- the order Matrix44::FromEulerAngles uses. */
        [[nodiscard]] static Quaternion FromEulerAngles(const Vector3<T>& radians) noexcept;

    public:
        /** The rotated basis. These are the columns of ToMatrix(). */
        [[nodiscard]] constexpr Vector3<T> Right() const noexcept;
        [[nodiscard]] constexpr Vector3<T> Up() const noexcept;
        [[nodiscard]] constexpr Vector3<T> Backward() const noexcept;
        [[nodiscard]] constexpr Vector3<T> Forward() const noexcept;

        [[nodiscard]] constexpr Matrix44<T> ToMatrix() const noexcept;
    };

    // Member functions
    template<Floating T>
    constexpr Quaternion<T>::Quaternion(T inX, T inY, T inZ, T inW) noexcept
        : x(inX)
        , y(inY)
        , z(inZ)
        , w(inW)
    {
    }

    template<Floating T>
    constexpr Quaternion<T> Quaternion<T>::operator*(const Quaternion& rhs) const noexcept
    {
        return Quaternion(w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
                          w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
                          w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
                          w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
    }

    template<Floating T>
    constexpr Quaternion<T>& Quaternion<T>::operator*=(const Quaternion& rhs) noexcept
    {
        return *this = *this * rhs;
    }

    template<Floating T>
    constexpr T Quaternion<T>::Dot(const Quaternion& other) const noexcept
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    template<Floating T>
    constexpr T Quaternion<T>::Length2() const noexcept
    {
        return Dot(*this);
    }

    template<Floating T>
    T Quaternion<T>::Length() const noexcept
    {
        return std::sqrt(Length2());
    }

    template<Floating T>
    void Quaternion<T>::Normalize() noexcept
    {
        if (const T length = Length(); length > kEpsilon<T>)
        {
            const T invLength = static_cast<T>(1) / length;
            x *= invLength;
            y *= invLength;
            z *= invLength;
            w *= invLength;
        }
    }

    template<Floating T>
    Quaternion<T> Quaternion<T>::Normalized() const noexcept
    {
        Quaternion result = *this;
        result.Normalize();
        return result;
    }

    template<Floating T>
    constexpr Quaternion<T> Quaternion<T>::Conjugate() const noexcept
    {
        return Quaternion(-x, -y, -z, w);
    }

    template<Floating T>
    constexpr Quaternion<T> Quaternion<T>::Inverse() const noexcept
    {
        const T length2 = Length2();
        if (length2 > kEpsilon<T>)
        {
            return Conjugate() * (static_cast<T>(1) / length2);
        }
        return *this;
    }

    template<Floating T>
    constexpr Quaternion<T> Quaternion<T>::Lerp(const Quaternion& start, const Quaternion& end, T t) noexcept
    {
        const T oneMinusT = static_cast<T>(1) - t;
        return Quaternion(start.x * oneMinusT + end.x * t,
                          start.y * oneMinusT + end.y * t,
                          start.z * oneMinusT + end.z * t,
                          start.w * oneMinusT + end.w * t);
    }

    template<Floating T>
    Quaternion<T> Quaternion<T>::Slerp(const Quaternion& start, const Quaternion& end, T t) noexcept
    {
        T dot = start.Dot(end);

        // q and -q are the same orientation: negate to take the shorter arc.
        Quaternion adjustedEnd = end;
        if (dot < static_cast<T>(0))
        {
            adjustedEnd = end * static_cast<T>(-1);
            dot = -dot;
        }

        // sin(theta) underflows as the arc closes, so fall back before dividing by it.
        if (dot > static_cast<T>(1) - kEpsilon<T>)
        {
            return Lerp(start, adjustedEnd, t).Normalized();
        }

        const T theta = std::acos(dot);
        const T sinTheta = std::sin(theta);

        const Quaternion scaledStart = start * (std::sin(theta * (static_cast<T>(1) - t)) / sinTheta);
        const Quaternion scaledEnd   = adjustedEnd * (std::sin(theta * t) / sinTheta);
        return scaledStart + scaledEnd;
    }

    template<Floating T>
    Quaternion<T> Quaternion<T>::FromAxisAngle(const Vector3<T>& axis, T radians) noexcept
    {
        Debug::Assert(AreValuesClose(axis.Length2(), static_cast<T>(1)), "Axis must be normalized");

        const T halfAngle = radians * static_cast<T>(0.5);
        const T sinHalf = std::sin(halfAngle);

        return Quaternion(axis.x * sinHalf, axis.y * sinHalf, axis.z * sinHalf, std::cos(halfAngle));
    }

    template<Floating T>
    Quaternion<T> Quaternion<T>::FromEulerAngles(const Vector3<T>& radians) noexcept
    {
        const T cx = std::cos(radians.x * static_cast<T>(0.5));
        const T sx = std::sin(radians.x * static_cast<T>(0.5));
        const T cy = std::cos(radians.y * static_cast<T>(0.5));
        const T sy = std::sin(radians.y * static_cast<T>(0.5));
        const T cz = std::cos(radians.z * static_cast<T>(0.5));
        const T sz = std::sin(radians.z * static_cast<T>(0.5));

        return Quaternion(sx * cy * cz + cx * sy * sz,
                          cx * sy * cz - sx * cy * sz,
                          cx * cy * sz + sx * sy * cz,
                          cx * cy * cz - sx * sy * sz);
    }

    template<Floating T>
    constexpr Vector3<T> Quaternion<T>::Right() const noexcept
    {
        return Vector3<T>(static_cast<T>(1) - static_cast<T>(2) * (y * y + z * z),
                          static_cast<T>(2) * (x * y + w * z),
                          static_cast<T>(2) * (x * z - w * y));
    }

    template<Floating T>
    constexpr Vector3<T> Quaternion<T>::Up() const noexcept
    {
        return Vector3<T>(static_cast<T>(2) * (x * y - w * z),
                          static_cast<T>(1) - static_cast<T>(2) * (x * x + z * z),
                          static_cast<T>(2) * (y * z + w * x));
    }

    template<Floating T>
    constexpr Vector3<T> Quaternion<T>::Backward() const noexcept
    {
        return Vector3<T>(static_cast<T>(2) * (x * z + w * y),
                          static_cast<T>(2) * (y * z - w * x),
                          static_cast<T>(1) - static_cast<T>(2) * (x * x + y * y));
    }

    template<Floating T>
    constexpr Vector3<T> Quaternion<T>::Forward() const noexcept
    {
        return -Backward();
    }

    template<Floating T>
    constexpr Matrix44<T> Quaternion<T>::ToMatrix() const noexcept
    {
        return Matrix44<T>(Vector4<T>(Right(),    static_cast<T>(0)),
                           Vector4<T>(Up(),       static_cast<T>(0)),
                           Vector4<T>(Backward(), static_cast<T>(0)),
                           Vector4<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)));
    }

    // Non-Member functions
    template<Floating T>
    [[nodiscard]] constexpr Quaternion<T> operator+(const Quaternion<T>& lhs, const Quaternion<T>& rhs) noexcept
    {
        return Quaternion<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
    }

    template<Floating T>
    [[nodiscard]] constexpr Quaternion<T> operator*(const Quaternion<T>& q, T scalar) noexcept
    {
        return Quaternion<T>(q.x * scalar, q.y * scalar, q.z * scalar, q.w * scalar);
    }

    template<Floating T>
    [[nodiscard]] constexpr Quaternion<T> operator*(T scalar, const Quaternion<T>& q) noexcept
    {
        return q * scalar;
    }

    template<Floating T>
    [[nodiscard]] constexpr Quaternion<T> operator/(const Quaternion<T>& q, T scalar) noexcept
    {
        return Quaternion<T>(q.x / scalar, q.y / scalar, q.z / scalar, q.w / scalar);
    }

    // Component-wise, so it separates q from -q. Compare ToMatrix() for orientation equality.
    template<Floating T>
    [[nodiscard]] constexpr bool AreValuesClose(const Quaternion<T>& a, const Quaternion<T>& b, T tolerance = kEpsilon<T>) noexcept
    {
        return AreValuesClose(a.x, b.x, tolerance) &&
               AreValuesClose(a.y, b.y, tolerance) &&
               AreValuesClose(a.z, b.z, tolerance) &&
               AreValuesClose(a.w, b.w, tolerance);
    }

    using Quat  = Quaternion<float32>;
    using Quatd = Quaternion<float64>;
}
