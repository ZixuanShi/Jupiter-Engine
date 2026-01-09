// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>
#include <type_traits>

export module jpt.Vector2;

import jpt.Constants;
import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Math;
import jpt.Hash;
import jpt.String;

export namespace jpt
{
    template<Numeric T>
    struct Vector2
    {
    public:
        T x = static_cast<T>(0);
        T y = static_cast<T>(0);

    public:
        [[nodiscard]] static consteval Vector2 Zero()  noexcept { return Vector2(static_cast<T>( 0), static_cast<T>(0 )); }
        [[nodiscard]] static consteval Vector2 One()   noexcept { return Vector2(static_cast<T>( 1), static_cast<T>(1 )); }
        [[nodiscard]] static consteval Vector2 Up()    noexcept { return Vector2(static_cast<T>( 0), static_cast<T>(1 )); }
        [[nodiscard]] static consteval Vector2 Down()  noexcept { return Vector2(static_cast<T>( 0), static_cast<T>(-1)); }
        [[nodiscard]] static consteval Vector2 Left()  noexcept { return Vector2(static_cast<T>(-1), static_cast<T>(0 )); }
        [[nodiscard]] static consteval Vector2 Right() noexcept { return Vector2(static_cast<T>( 1), static_cast<T>(0 )); }

    public:
        constexpr Vector2() = default;
        constexpr Vector2(T val);
        constexpr Vector2(T x, T y);

    public:
        constexpr Vector2& operator+=(Vector2 other) noexcept;
        constexpr Vector2& operator-=(Vector2 other) noexcept;
        constexpr Vector2& operator*=(Vector2 other) noexcept;
        constexpr Vector2& operator/=(Vector2 other) noexcept;
        constexpr Vector2& operator+=(T scalar) noexcept;
        constexpr Vector2& operator-=(T scalar) noexcept;
        constexpr Vector2& operator*=(T scalar) noexcept;
        constexpr Vector2& operator/=(T scalar) noexcept;

    public:
        [[nodiscard]] constexpr T Dot(Vector2 other) const noexcept;

        [[nodiscard]] constexpr T Length()  const noexcept;
        [[nodiscard]] constexpr T Length2() const noexcept;  // 2 means squared

        [[nodiscard]] constexpr T Distance(Vector2 other) const noexcept;
        [[nodiscard]] constexpr T Distance2(Vector2 other) const noexcept; // 2 means squared

        constexpr void Normalize() noexcept;
        [[nodiscard]] constexpr Vector2 Normalized() const noexcept;

        [[nodiscard]] constexpr static T Angle(Vector2 lhs, Vector2 rhs) noexcept;         // Unsigned Radians. Faster (no atan2
        [[nodiscard]] constexpr static T AngleSigned(Vector2 from, Vector2 to) noexcept;   // Signed Radians. Slower
        
        // Counter-clockwise rotation
        [[nodiscard]] constexpr Vector2 Rotate(T radians) noexcept;
        [[nodiscard]] constexpr Vector2 RotateAround(Vector2 pivot, T radians) noexcept;
    };

    // ------------------------------------------------------------------------------------------------
    // Non-Member functions
    // ------------------------------------------------------------------------------------------------
    template<Numeric T>
    [[nodiscard]] constexpr bool operator==(const Vector2<T>& lhs, const Vector2<T>& rhs)
    {
        return AreValuesClose(lhs.x, rhs.x) &&
               AreValuesClose(lhs.y, rhs.y);
    }
    
    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator-(Vector2<T> vector2) noexcept
    {
        return Vector2(-vector2.x, -vector2.y);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator+(Vector2<T> lhs, Vector2<T> rhs) noexcept
    {
        return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator-(Vector2<T> lhs, Vector2<T> rhs) noexcept
    {
        return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator*(Vector2<T> lhs, Vector2<T> rhs) noexcept
    {
        return Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator/(Vector2<T> lhs, Vector2<T> rhs) noexcept
    {
        return Vector2(lhs.x / rhs.x, lhs.y / rhs.y);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator+(Vector2<T> vector2, T scalar) noexcept
    {
        return Vector2(vector2.x + scalar, vector2.y + scalar);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator-(Vector2<T> vector2, T scalar) noexcept
    {
        return Vector2(scalar - vector2.x, scalar - vector2.y);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator*(Vector2<T> vector2, T scalar) noexcept
    {
        return Vector2(vector2.x * scalar, vector2.y * scalar);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator/(Vector2<T> vector2, T scalar) noexcept
    {
        return Vector2(vector2.x / scalar, vector2.y / scalar);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator+(T scaler, Vector2<T> vector2)
    {
        return vector2 + scaler;
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator-(T scaler, Vector2<T> vector2)
    {
        return vector2 - scaler;
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator*(T scaler, Vector2<T> vector2)
    {
        return vector2 * scaler;
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2<T> operator/(T scaler, Vector2<T> vector2)
    {
        return vector2 / scaler;
    }

    template<Numeric T>
    [[nodiscard]] constexpr uint64 Hash(const Vector2<T>& vector2)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            const T epsilon = static_cast<T>(1e-6);
            auto round = [epsilon](T value) -> T
                {
                    return (value < epsilon && value > -epsilon) ? static_cast<T>(0) : value;
                };

            uint64 hash = jpt::Hash(round(vector2.x));
            hash ^= jpt::Hash(round(vector2.y)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
        else
        {
            uint64 hash = jpt::Hash(vector2.x);
            hash ^= jpt::Hash(vector2.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
    }

    template<Numeric T>
    [[nodiscard]] constexpr String ToString(const Vector2<T>& vector2)
    {
        return String::Format<32>("x: %.3f, y: %.3f", vector2.x, vector2.y);
    }

    // ------------------------------------------------------------------------------------------------
    // Member functions
    // ------------------------------------------------------------------------------------------------
    template<Numeric T>
    constexpr Vector2<T>::Vector2(T val)
        : x(val)
        , y(val)
    {
    }

    template<Numeric T>
    constexpr Vector2<T>::Vector2(T x, T y)
        : x(x)
        , y(y)
    {
    }

    template<Numeric T>
    constexpr Vector2<T> operator-(const Vector2<T>& vector2) noexcept
    {
        return Vector2(-vector2.x, -vector2.y);
    }

    template<Numeric T>
    constexpr Vector2<T>& Vector2<T>::operator+=(Vector2 other) noexcept
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<Numeric T>
    constexpr Vector2<T>& Vector2<T>::operator-=(Vector2 other) noexcept
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    template<Numeric T>
    constexpr Vector2<T>& Vector2<T>::operator*=(Vector2 other) noexcept
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    template<Numeric T>
    constexpr Vector2<T>& Vector2<T>::operator/=(Vector2 other) noexcept
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    template<Numeric T>
    constexpr Vector2<T>& Vector2<T>::operator+=(T scalar) noexcept
    {
        x += scalar; 
        y += scalar; 
        return *this;
    }

    template<Numeric T>
    constexpr Vector2<T>& Vector2<T>::operator-=(T scalar) noexcept
    {
        x -= scalar;
        y -= scalar; 
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
    constexpr T Vector2<T>::Dot(Vector2 other) const noexcept
    {
        return (x * other.x) + (y * other.y);
    }

    template<Numeric T>
    constexpr T Vector2<T>::Length() const noexcept
    {
        return std::sqrt(Length2());
    }

    template<Numeric T>
    constexpr T Vector2<T>::Length2() const noexcept
    {
        return (x * x) + (y * y);
    }

    template<Numeric T>
    constexpr T Vector2<T>::Distance(Vector2 other) const noexcept
    {
        return Sqrt(Distance2(other));
    }

    template<Numeric T>
    constexpr T Vector2<T>::Distance2(Vector2 other) const noexcept
    {
        return (x - other.x) * (x - other.x) + 
               (y - other.y) * (y - other.y);
    }

    template<Numeric T>
    constexpr void Vector2<T>::Normalize() noexcept
    {
        const T length = Length();
        if (length != 0.0f)
        {
            x /= length;
            y /= length;
        }
    }

    template<Numeric T>
    constexpr Vector2<T> Vector2<T>::Normalized() const noexcept
    {
        Vector2<T> result = *this;
        result.Normalize();
        return result;
    }

    template<Numeric T>
    constexpr T Vector2<T>::Angle(Vector2 from, Vector2 to) noexcept
    {
        // Formular: dot(a, b) / (|a| * |b|)
        // Assumes from and to are normalized

        const T dot = from.Dot(to);
        const T angle = Acos(dot);

        return angle;
    }

    template<Numeric T>
    constexpr T Vector2<T>::AngleSigned(Vector2 from, Vector2 to) noexcept
    {
        T atan2 = Atan2(to.y, to.x) - Atan2(from.y, from.x);

        if (atan2 > kPi<T>)
        {
            atan2 -= kTwoPi<T>;
        }
        else if (atan2 < -kPi<T>)
        {
            atan2 += kTwoPi<T>;
        }

        return atan2;
    }

    template<Numeric T>
    constexpr Vector2<T> Vector2<T>::Rotate(T radians) noexcept
    {
        Vector2<T> v = *this;

        const T cos = Cos(radians);
        const T sin = Sin(radians);

        v.x = x * cos - y * sin;
        v.y = x * sin + y * cos;

        return v;
    }

    template<Numeric T>
    constexpr Vector2<T> Vector2<T>::RotateAround(Vector2 pivot, T radians) noexcept
    {
        Vector2<T> v = *this;

        const T cos = Cos(radians);
        const T sin = Sin(radians);

        const Vector2<T> temp = *this - pivot;

        v.x = temp.x * cos - temp.y * sin + pivot.x;
        v.y = temp.x * sin + temp.y * cos + pivot.y;

        return v;
    }
}

export using Vec2 = jpt::Vector2<Precision>;
export using Vec2f = jpt::Vector2<float32>;
export using Vec2d = jpt::Vector2<float64>;
export using Vec2i = jpt::Vector2<int32>;
export using Vec2u = jpt::Vector2<uint32>;

template<> constexpr bool jpt::IsTrivial<Vec2f> = true;
template<> constexpr bool jpt::IsTrivial<Vec2d> = true;
template<> constexpr bool jpt::IsTrivial<Vec2i> = true;
template<> constexpr bool jpt::IsTrivial<Vec2u> = true;