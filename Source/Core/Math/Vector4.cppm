// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Vector4;

import jpt.Concepts;
import jpt.Constants;
import jpt.Math;
import jpt.TypeDefs;
import jpt.Vector3;
import std;

export namespace jpt
{
    template<Numeric T>
    struct Vector4
    {
    public:
        T x = static_cast<T>(0);
        T y = static_cast<T>(0);
        T z = static_cast<T>(0);
        T w = static_cast<T>(0);

    public:
        [[nodiscard]] static consteval Vector4 Zero() noexcept { return Vector4(static_cast<T>(0)); }
        [[nodiscard]] static consteval Vector4 One()  noexcept { return Vector4(static_cast<T>(1)); }

    public:
        constexpr Vector4() noexcept = default;
        constexpr Vector4(T scalar) noexcept;
        constexpr Vector4(T inX, T inY, T inZ, T inW) noexcept;
        constexpr Vector4(const Vector3<T>& vector3, T inW) noexcept;

    public:
        [[nodiscard]] constexpr T&       operator[](usize index) noexcept;
        [[nodiscard]] constexpr const T& operator[](usize index) const noexcept;

        [[nodiscard]] constexpr Vector4 operator-() const noexcept;

        [[nodiscard]] constexpr Vector4 operator+(const Vector4& other) const noexcept;
        [[nodiscard]] constexpr Vector4 operator-(const Vector4& other) const noexcept;
        [[nodiscard]] constexpr Vector4 operator*(T scalar) const noexcept;
        [[nodiscard]] constexpr Vector4 operator/(T scalar) const noexcept;

        constexpr Vector4& operator+=(const Vector4& other) noexcept;
        constexpr Vector4& operator-=(const Vector4& other) noexcept;
        constexpr Vector4& operator*=(T scalar) noexcept;
        constexpr Vector4& operator/=(T scalar) noexcept;

        [[nodiscard]] constexpr bool operator==(const Vector4& other) const noexcept = default;

    public:
        [[nodiscard]] constexpr Vector3<T> XYZ() const noexcept;

        [[nodiscard]] constexpr T Dot(const Vector4& other) const noexcept;
        [[nodiscard]] constexpr T Length2() const noexcept;
        [[nodiscard]] T Length() const noexcept requires Floating<T>;
    };

    // Non-Member functions
    template<Numeric T>
    [[nodiscard]] constexpr Vector4<T> operator*(T scalar, const Vector4<T>& vector4) noexcept
    {
        return vector4 * scalar;
    }

    template<Floating T>
    [[nodiscard]] constexpr bool AreValuesClose(const Vector4<T>& a, const Vector4<T>& b, T tolerance = kEpsilon<T>) noexcept
    {
        return AreValuesClose(a.x, b.x, tolerance) &&
               AreValuesClose(a.y, b.y, tolerance) &&
               AreValuesClose(a.z, b.z, tolerance) &&
               AreValuesClose(a.w, b.w, tolerance);
    }

    // Member functions
    template<Numeric T>
    constexpr Vector4<T>::Vector4(T scalar) noexcept
        : x(scalar)
        , y(scalar)
        , z(scalar)
        , w(scalar)
    {
    }

    template<Numeric T>
    constexpr Vector4<T>::Vector4(T inX, T inY, T inZ, T inW) noexcept
        : x(inX)
        , y(inY)
        , z(inZ)
        , w(inW)
    {
    }

    template<Numeric T>
    constexpr Vector4<T>::Vector4(const Vector3<T>& vector3, T inW) noexcept
        : x(vector3.x)
        , y(vector3.y)
        , z(vector3.z)
        , w(inW)
    {
    }

    template<Numeric T>
    constexpr T& Vector4<T>::operator[](usize index) noexcept
    {
        switch (index)
        {
            case 0:  return x;
            case 1:  return y;
            case 2:  return z;
            default: return w;
        }
    }

    template<Numeric T>
    constexpr const T& Vector4<T>::operator[](usize index) const noexcept
    {
        switch (index)
        {
            case 0:  return x;
            case 1:  return y;
            case 2:  return z;
            default: return w;
        }
    }

    template<Numeric T>
    constexpr Vector4<T> Vector4<T>::operator-() const noexcept
    {
        return Vector4(-x, -y, -z, -w);
    }

    template<Numeric T>
    constexpr Vector4<T> Vector4<T>::operator+(const Vector4& other) const noexcept
    {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    template<Numeric T>
    constexpr Vector4<T> Vector4<T>::operator-(const Vector4& other) const noexcept
    {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    template<Numeric T>
    constexpr Vector4<T> Vector4<T>::operator*(T scalar) const noexcept
    {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    template<Numeric T>
    constexpr Vector4<T> Vector4<T>::operator/(T scalar) const noexcept
    {
        return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    template<Numeric T>
    constexpr Vector4<T>& Vector4<T>::operator+=(const Vector4& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    template<Numeric T>
    constexpr Vector4<T>& Vector4<T>::operator-=(const Vector4& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    template<Numeric T>
    constexpr Vector4<T>& Vector4<T>::operator*=(T scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    template<Numeric T>
    constexpr Vector4<T>& Vector4<T>::operator/=(T scalar) noexcept
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    template<Numeric T>
    constexpr Vector3<T> Vector4<T>::XYZ() const noexcept
    {
        return Vector3<T>(x, y, z);
    }

    template<Numeric T>
    constexpr T Vector4<T>::Dot(const Vector4& other) const noexcept
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    template<Numeric T>
    constexpr T Vector4<T>::Length2() const noexcept
    {
        return Dot(*this);
    }

    template<Numeric T>
    T Vector4<T>::Length() const noexcept requires Floating<T>
    {
        return std::sqrt(Length2());
    }

    using Vec4  = Vector4<float32>;
    using Vec4d = Vector4<float64>;
    using Vec4i = Vector4<int32>;
}
