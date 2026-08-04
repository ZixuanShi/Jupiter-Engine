// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Vector2;

import jpt.Concepts;
import jpt.Constants;
import jpt.Math;
import jpt.TypeDefs;
import std;

export namespace jpt
{
    template<Numeric T>
    struct Vector2
    {
    public:
        T x = static_cast<T>(0);
        T y = static_cast<T>(0);

    public:
        [[nodiscard]] static consteval Vector2 Zero() noexcept { return Vector2(static_cast<T>(0)); }
        [[nodiscard]] static consteval Vector2 One()  noexcept { return Vector2(static_cast<T>(1)); }

        [[nodiscard]] static consteval Vector2 Right() noexcept { return Vector2(static_cast<T>( 1), static_cast<T>( 0)); }
        [[nodiscard]] static consteval Vector2 Left()  noexcept { return Vector2(static_cast<T>(-1), static_cast<T>( 0)); }
        [[nodiscard]] static consteval Vector2 Up()    noexcept { return Vector2(static_cast<T>( 0), static_cast<T>( 1)); }
        [[nodiscard]] static consteval Vector2 Down()  noexcept { return Vector2(static_cast<T>( 0), static_cast<T>(-1)); }

    public:
        constexpr Vector2() noexcept = default;
        constexpr Vector2(T scalar) noexcept;
        constexpr Vector2(T inX, T inY) noexcept;

    public:
        [[nodiscard]] constexpr Vector2 operator-() const noexcept;

        [[nodiscard]] constexpr Vector2 operator+(const Vector2& other) const noexcept;
        [[nodiscard]] constexpr Vector2 operator-(const Vector2& other) const noexcept;
        [[nodiscard]] constexpr Vector2 operator*(T scalar) const noexcept;
        [[nodiscard]] constexpr Vector2 operator/(T scalar) const noexcept;

        constexpr Vector2& operator+=(const Vector2& other) noexcept;
        constexpr Vector2& operator-=(const Vector2& other) noexcept;
        constexpr Vector2& operator*=(T scalar) noexcept;
        constexpr Vector2& operator/=(T scalar) noexcept;

        [[nodiscard]] constexpr bool operator==(const Vector2& other) const noexcept = default;

    public:
        [[nodiscard]] constexpr T Dot(const Vector2& other) const noexcept;

        [[nodiscard]] constexpr T Length2() const noexcept;
        [[nodiscard]] T Length() const noexcept requires Floating<T>;
        [[nodiscard]] T Distance(const Vector2& other) const noexcept requires Floating<T>;

        /** Guards against kEpsilon rather than exact zero: a denormal length divides to inf. */
        void Normalize() noexcept requires Floating<T>;
        [[nodiscard]] Vector2 Normalized() const noexcept requires Floating<T>;
    };

    // Non-Member functions
    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator*(T scalar, const Vector2<T>& vector2) noexcept
    {
        return vector2 * scalar;
    }

    template<Floating T>
    [[nodiscard]] constexpr bool AreValuesClose(const Vector2<T>& a, const Vector2<T>& b, T tolerance = kEpsilon<T>) noexcept
    {
        return AreValuesClose(a.x, b.x, tolerance) &&
               AreValuesClose(a.y, b.y, tolerance);
    }

    // Member functions
    template<Numeric T>
    constexpr Vector2<T>::Vector2(T scalar) noexcept
        : x(scalar)
        , y(scalar)
    {
    }

    template<Numeric T>
    constexpr Vector2<T>::Vector2(T inX, T inY) noexcept
        : x(inX)
        , y(inY)
    {
    }

    template<Numeric T>
    constexpr Vector2<T> Vector2<T>::operator-() const noexcept
    {
        return Vector2(-x, -y);
    }

    template<Numeric T>
    constexpr Vector2<T> Vector2<T>::operator+(const Vector2& other) const noexcept
    {
        return Vector2(x + other.x, y + other.y);
    }

    template<Numeric T>
    constexpr Vector2<T> Vector2<T>::operator-(const Vector2& other) const noexcept
    {
        return Vector2(x - other.x, y - other.y);
    }

    template<Numeric T>
    constexpr Vector2<T> Vector2<T>::operator*(T scalar) const noexcept
    {
        return Vector2(x * scalar, y * scalar);
    }

    template<Numeric T>
    constexpr Vector2<T> Vector2<T>::operator/(T scalar) const noexcept
    {
        return Vector2(x / scalar, y / scalar);
    }

    template<Numeric T>
    constexpr Vector2<T>& Vector2<T>::operator+=(const Vector2& other) noexcept
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<Numeric T>
    constexpr Vector2<T>& Vector2<T>::operator-=(const Vector2& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    template<Numeric T>
    constexpr Vector2<T>& Vector2<T>::operator*=(T scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    template<Numeric T>
    constexpr Vector2<T>& Vector2<T>::operator/=(T scalar) noexcept
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    template<Numeric T>
    constexpr T Vector2<T>::Dot(const Vector2& other) const noexcept
    {
        return x * other.x + y * other.y;
    }

    template<Numeric T>
    constexpr T Vector2<T>::Length2() const noexcept
    {
        return Dot(*this);
    }

    template<Numeric T>
    T Vector2<T>::Length() const noexcept requires Floating<T>
    {
        return std::sqrt(Length2());
    }

    template<Numeric T>
    T Vector2<T>::Distance(const Vector2& other) const noexcept requires Floating<T>
    {
        return (*this - other).Length();
    }

    template<Numeric T>
    void Vector2<T>::Normalize() noexcept requires Floating<T>
    {
        if (const T length = Length(); length > kEpsilon<T>)
        {
            *this /= length;
        }
    }

    template<Numeric T>
    Vector2<T> Vector2<T>::Normalized() const noexcept requires Floating<T>
    {
        Vector2 result = *this;
        result.Normalize();
        return result;
    }

    using Vec2  = Vector2<float32>;
    using Vec2d = Vector2<float64>;
    using Vec2i = Vector2<int32>;
}
