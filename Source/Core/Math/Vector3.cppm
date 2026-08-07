// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Vector3;

import jpt.Concepts;
import jpt.Constants;
import jpt.Math;
import jpt.TypeDefs;
import std;

export namespace jpt
{
    template<Numeric T>
    struct Vector3
    {
    public:
        [[nodiscard]] static consteval Vector3 Zero() noexcept { return Vector3(static_cast<T>(0)); }
        [[nodiscard]] static consteval Vector3 One()  noexcept { return Vector3(static_cast<T>(1)); }

        /** Right-handed, Y up, forward -Z */
        [[nodiscard]] static consteval Vector3 Right()    noexcept { return Vector3(static_cast<T>( 1), static_cast<T>( 0), static_cast<T>( 0)); }
        [[nodiscard]] static consteval Vector3 Left()     noexcept { return Vector3(static_cast<T>(-1), static_cast<T>( 0), static_cast<T>( 0)); }
        [[nodiscard]] static consteval Vector3 Up()       noexcept { return Vector3(static_cast<T>( 0), static_cast<T>( 1), static_cast<T>( 0)); }
        [[nodiscard]] static consteval Vector3 Down()     noexcept { return Vector3(static_cast<T>( 0), static_cast<T>(-1), static_cast<T>( 0)); }
        [[nodiscard]] static consteval Vector3 Forward()  noexcept { return Vector3(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>(-1)); }
        [[nodiscard]] static consteval Vector3 Backward() noexcept { return Vector3(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>( 1)); }

    public:
        T x = static_cast<T>(0);    /**< i, right */
        T y = static_cast<T>(0);    /**< j, up */
        T z = static_cast<T>(0);    /**< k, backward -- forward is -z */

    public:
        constexpr Vector3() noexcept = default;
        constexpr Vector3(T scalar) noexcept;
        constexpr Vector3(T inX, T inY, T inZ) noexcept;

    public:
        [[nodiscard]] constexpr Vector3 operator-() const noexcept;

        [[nodiscard]] constexpr Vector3 operator+(const Vector3& other) const noexcept;
        [[nodiscard]] constexpr Vector3 operator-(const Vector3& other) const noexcept;
        [[nodiscard]] constexpr Vector3 operator*(T scalar) const noexcept;
        [[nodiscard]] constexpr Vector3 operator/(T scalar) const noexcept;

        constexpr Vector3& operator+=(const Vector3& other) noexcept;
        constexpr Vector3& operator-=(const Vector3& other) noexcept;
        constexpr Vector3& operator*=(T scalar) noexcept;
        constexpr Vector3& operator/=(T scalar) noexcept;

        [[nodiscard]] constexpr bool operator==(const Vector3& other) const noexcept = default;

    public:
        [[nodiscard]] constexpr T Dot(const Vector3& other) const noexcept;

        /** Right-handed: Right().Cross(Up()) == Backward(), so the basis follows x cross y = z. */
        [[nodiscard]] constexpr Vector3 Cross(const Vector3& other) const noexcept;

        [[nodiscard]] constexpr T Length2() const noexcept;
        [[nodiscard]] T Length() const noexcept requires Floating<T>;
        [[nodiscard]] T Distance(const Vector3& other) const noexcept requires Floating<T>;

        /** Guards against kEpsilon rather than exact zero: a denormal length divides to inf. */
        void Normalize() noexcept requires Floating<T>;
        [[nodiscard]] Vector3 Normalized() const noexcept requires Floating<T>;
    };

    // Member functions
    template<Numeric T>
    constexpr Vector3<T>::Vector3(T scalar) noexcept
        : x(scalar)
        , y(scalar)
        , z(scalar)
    {
    }

    template<Numeric T>
    constexpr Vector3<T>::Vector3(T inX, T inY, T inZ) noexcept
        : x(inX)
        , y(inY)
        , z(inZ)
    {
    }

    template<Numeric T>
    constexpr Vector3<T> Vector3<T>::operator-() const noexcept
    {
        return Vector3(-x, -y, -z);
    }

    template<Numeric T>
    constexpr Vector3<T> Vector3<T>::operator+(const Vector3& other) const noexcept
    {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    template<Numeric T>
    constexpr Vector3<T> Vector3<T>::operator-(const Vector3& other) const noexcept
    {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    template<Numeric T>
    constexpr Vector3<T> Vector3<T>::operator*(T scalar) const noexcept
    {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    template<Numeric T>
    constexpr Vector3<T> Vector3<T>::operator/(T scalar) const noexcept
    {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    template<Numeric T>
    constexpr Vector3<T>& Vector3<T>::operator+=(const Vector3& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    template<Numeric T>
    constexpr Vector3<T>& Vector3<T>::operator-=(const Vector3& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    template<Numeric T>
    constexpr Vector3<T>& Vector3<T>::operator*=(T scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    template<Numeric T>
    constexpr Vector3<T>& Vector3<T>::operator/=(T scalar) noexcept
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    template<Numeric T>
    constexpr T Vector3<T>::Dot(const Vector3& other) const noexcept
    {
        return x * other.x + y * other.y + z * other.z;
    }

    template<Numeric T>
    constexpr Vector3<T> Vector3<T>::Cross(const Vector3& other) const noexcept
    {
        return Vector3(y * other.z - z * other.y,
                       z * other.x - x * other.z,
                       x * other.y - y * other.x);
    }

    template<Numeric T>
    constexpr T Vector3<T>::Length2() const noexcept
    {
        return Dot(*this);
    }

    template<Numeric T>
    T Vector3<T>::Length() const noexcept requires Floating<T>
    {
        return std::sqrt(Length2());
    }

    template<Numeric T>
    T Vector3<T>::Distance(const Vector3& other) const noexcept requires Floating<T>
    {
        return (*this - other).Length();
    }

    template<Numeric T>
    void Vector3<T>::Normalize() noexcept requires Floating<T>
    {
        if (const T length = Length(); length > kEpsilon<T>)
        {
            *this /= length;
        }
    }

    template<Numeric T>
    Vector3<T> Vector3<T>::Normalized() const noexcept requires Floating<T>
    {
        Vector3 result = *this;
        result.Normalize();
        return result;
    }

    // Non-Member functions
    template<Numeric T>
    [[nodiscard]] constexpr Vector3<T> operator*(T scalar, const Vector3<T>& vector3) noexcept
    {
        return vector3 * scalar;
    }

    template<Floating T>
    [[nodiscard]] constexpr bool AreValuesClose(const Vector3<T>& a, const Vector3<T>& b, T tolerance = kEpsilon<T>) noexcept
    {
        return AreValuesClose(a.x, b.x, tolerance) &&
               AreValuesClose(a.y, b.y, tolerance) &&
               AreValuesClose(a.z, b.z, tolerance);
    }

    using Vec3  = Vector3<float32>;
    using Vec3d = Vector3<float64>;
    using Vec3i = Vector3<int32>;
}
