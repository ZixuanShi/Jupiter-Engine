// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Vector4;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Math;
import jpt.String;
import jpt.Vector3;
import jpt.Hash;

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
        [[nodiscard]] static consteval Vector4 Zero()     noexcept { return Vector4(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>( 0), static_cast<T>(0)); }
        [[nodiscard]] static consteval Vector4 One()      noexcept { return Vector4(static_cast<T>( 1), static_cast<T>( 1), static_cast<T>( 1), static_cast<T>(1)); }
        [[nodiscard]] static consteval Vector4 Up()       noexcept { return Vector4(static_cast<T>( 0), static_cast<T>( 1), static_cast<T>( 0), static_cast<T>(0)); }
        [[nodiscard]] static consteval Vector4 Down()     noexcept { return Vector4(static_cast<T>( 0), static_cast<T>(-1), static_cast<T>( 0), static_cast<T>(0)); }
        [[nodiscard]] static consteval Vector4 Left()     noexcept { return Vector4(static_cast<T>(-1), static_cast<T>( 0), static_cast<T>( 0), static_cast<T>(0)); }
        [[nodiscard]] static consteval Vector4 Right()    noexcept { return Vector4(static_cast<T>( 1), static_cast<T>( 0), static_cast<T>( 0), static_cast<T>(0)); }
        [[nodiscard]] static consteval Vector4 Forward()  noexcept { return Vector4(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>( 1), static_cast<T>(0)); }
        [[nodiscard]] static consteval Vector4 Backward() noexcept { return Vector4(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>(-1), static_cast<T>(0)); }

    public:
        constexpr Vector4() = default;
        constexpr Vector4(T scalar);
        constexpr Vector4(T x, T y, T z, T w);
        constexpr Vector4(const Vector3<T>& vector, T w);

    public:
        constexpr Vector4& operator+=(const Vector4& other) noexcept;
        constexpr Vector4& operator-=(const Vector4& other) noexcept;
        constexpr Vector4& operator*=(const Vector4& other) noexcept;
        constexpr Vector4& operator/=(const Vector4& other) noexcept;
        constexpr Vector4& operator+=(T scalar) noexcept;
        constexpr Vector4& operator-=(T scalar) noexcept;
        constexpr Vector4& operator*=(T scalar) noexcept;
        constexpr Vector4& operator/=(T scalar) noexcept;

        [[nodiscard]] constexpr       T& operator[](size_t index) noexcept;
        [[nodiscard]] constexpr const T& operator[](size_t index) const noexcept;

    public:
        [[nodiscard]] constexpr T Length()   const noexcept;
        [[nodiscard]] constexpr T Length2()  const noexcept;
        [[nodiscard]] constexpr T LengthW()  const noexcept;
        [[nodiscard]] constexpr T Length2W() const noexcept;

        [[nodiscard]] constexpr T Distance(const Vector4& other)  const noexcept;
        [[nodiscard]] constexpr T Distance2(const Vector4& other) const noexcept;

        [[nodiscard]] constexpr T Dot(const Vector4& other) const noexcept;
        [[nodiscard]] constexpr T DotW(const Vector4& other) const noexcept;
        [[nodiscard]] constexpr Vector3<T> Cross(const Vector4& other) const noexcept;
        
        [[nodiscard]] constexpr Vector3<T> XYZ() const noexcept;

        // Homogeneous coordinates
        [[nodiscard]] constexpr bool IsDir() const noexcept;
        [[nodiscard]] constexpr bool IsPos() const noexcept;
    };

    // ------------------------------------------------------------------------------------------------
    // Non-Member functions
    // ------------------------------------------------------------------------------------------------
    template<Numeric T>
    [[nodiscard]] constexpr bool operator==(const Vector4<T>& lhs, const Vector4<T>& rhs)
    {
        return AreValuesClose(lhs.x, rhs.x) &&
               AreValuesClose(lhs.y, rhs.y) &&
               AreValuesClose(lhs.z, rhs.z) &&
               AreValuesClose(lhs.w, rhs.w);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector4<T> operator-(const Vector4<T>& vector4) noexcept
    {
        return Vector4(-vector4.x, -vector4.y, -vector4.z, -vector4.w);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector4<T> operator+(const Vector4<T>& lhs, const Vector4<T>& rhs) noexcept
    {
        return Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector4<T> operator-(const Vector4<T>& lhs, const Vector4<T>& rhs) noexcept
    {
        return Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector4<T> operator*(const Vector4<T>& lhs, const Vector4<T>& rhs) noexcept
    {
        return Vector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector4<T> operator/(const Vector4<T>& lhs, const Vector4<T>& rhs) noexcept
    {
        return Vector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector4<T> operator+(const Vector4<T>& vector4, T scalar) noexcept
    {
        return Vector4(vector4.x + scalar, vector4.y + scalar, vector4.z + scalar, vector4.w + scalar);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector4<T> operator-(const Vector4<T>& vector4, T scalar) noexcept
    {
        return Vector4(vector4.x - scalar, vector4.y - scalar, vector4.z - scalar, vector4.w - scalar);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector4<T> operator*(const Vector4<T>& vector4, T scalar) noexcept
    {
        return Vector4(vector4.x * scalar, vector4.y * scalar, vector4.z * scalar, vector4.w * scalar);
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector4<T> operator/(const Vector4<T>& vector4, T scalar) noexcept
    {
        return Vector4(vector4.x / scalar, vector4.y / scalar, vector4.z / scalar, vector4.w / scalar);
    }
    
    template<Numeric T>
    [[nodiscard]] constexpr Vector4<T> operator+(T scaler, const Vector4<T>& vector4) noexcept
    {
        return vector4 + scaler;
    }
    
    template<Numeric T>
    [[nodiscard]] constexpr Vector4<T> operator-(T scaler, const Vector4<T>& vector4) noexcept
    {
        return vector4 - scaler;
    }
    
    template<Numeric T>
    [[nodiscard]] constexpr Vector4<T> operator*(T scaler, const Vector4<T>& vector4) noexcept
    {
        return vector4 * scaler;
    }
    
    template<Numeric T>
    [[nodiscard]] constexpr Vector4<T> operator/(T scaler, const Vector4<T>& vector4) noexcept
    {
        return vector4 / scaler;
    }

    template<Numeric T>
    [[nodiscard]] constexpr uint64 Hash(const Vector4<T>& vector4)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            const T epsilon = static_cast<T>(1e-6);
            auto round = [epsilon](T value) -> T
                {
                    return (value < epsilon && value > -epsilon) ? static_cast<T>(0) : value;
                };

            uint64 hash = jpt::Hash(round(vector4.x));
            hash ^= jpt::Hash(round(vector4.y)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= jpt::Hash(round(vector4.z)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= jpt::Hash(round(vector4.w)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
        else
        {
            uint64 hash = jpt::Hash(vector4.x);
            hash ^= jpt::Hash(vector4.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= jpt::Hash(vector4.z) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= jpt::Hash(vector4.w) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
    }

    template<Numeric T>
    [[nodiscard]] constexpr String ToString(const Vector4<T>& vector4)
    {
        return String::Format<64>("x: %.3f, y: %.3f, w: %.3f, w: %.3f", vector4.x, vector4.y, vector4.z, vector4.w);
    }

    // ------------------------------------------------------------------------------------------------
    // Member functions
    // ------------------------------------------------------------------------------------------------
    template<Numeric T>
    constexpr Vector4<T>::Vector4(T scalar)
        : x(scalar)
        , y(scalar)
        , z(scalar)
        , w(static_cast<T>(1)) 
    {
    }

    template<Numeric T>
    constexpr Vector4<T>::Vector4(T x, T y, T z, T w)
        : x(x)
        , y(y)
        , z(z)
        , w(w) 
    {
    }

    template<Numeric T>
    constexpr Vector4<T>::Vector4(const Vector3<T>& vector, T w)
        : x(vector.x)
        , y(vector.y)
        , z(vector.z)
        , w(w) 
    {
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
    constexpr Vector4<T>& Vector4<T>::operator*=(const Vector4& other) noexcept
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
        return *this;
    }

    template<Numeric T>
    constexpr Vector4<T>& Vector4<T>::operator/=(const Vector4& other) noexcept
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
        return *this;
    }

    template<Numeric T>
    constexpr Vector4<T>& Vector4<T>::operator+=(T scalar) noexcept
    {
        x += scalar;
        y += scalar;
        z += scalar;
        w += scalar;
        return *this;
    }

    template<Numeric T>
    constexpr Vector4<T>& Vector4<T>::operator-=(T scalar) noexcept
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        w -= scalar;
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
    constexpr T& Vector4<T>::operator[](size_t index) noexcept
    {
        return (&x)[index];
    }

    template<Numeric T>
    constexpr const T& Vector4<T>::operator[](size_t index) const noexcept
    {
        return (&x)[index];
    }

    template<Numeric T>
    constexpr T Vector4<T>::Length() const noexcept
    {
        return Sqrt(Length2());
    }

    template<Numeric T>
    constexpr T Vector4<T>::LengthW() const noexcept
    {
        return Sqrt(Length2W());
    }

    template<Numeric T>
    constexpr T Vector4<T>::Length2() const noexcept
    {
        return x * x + y * y + z * z;
    }

    template<Numeric T>
    constexpr T Vector4<T>::Length2W() const noexcept
    {
        return x * x + y * y + z * z + w * w;
    }

    template<Numeric T>
    constexpr T Vector4<T>::Distance(const Vector4& other) const noexcept
    {
        return Sqrt(Distance2(other));
    }

    template<Numeric T>
    constexpr T Vector4<T>::Distance2(const Vector4& other) const noexcept
    {
        return (x - other.x) * (x - other.x) + 
               (y - other.y) * (y - other.y) + 
               (z - other.z) * (z - other.z);
    }

    template<Numeric T>
    constexpr T Vector4<T>::Dot(const Vector4& other) const noexcept
    {
        return XYZ().Dot(other.XYZ());
    }

    template<Numeric T>
    constexpr T Vector4<T>::DotW(const Vector4& other) const noexcept
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    template<Numeric T>
    constexpr Vector3<T> Vector4<T>::Cross(const Vector4& other) const noexcept
    {
        return XYZ().Cross(other.XYZ());
    }

    template<Numeric T>
    constexpr Vector3<T> Vector4<T>::XYZ() const noexcept
    {
        return Vector3<T>(x, y, z);
    }

    template<Numeric T>
    constexpr bool Vector4<T>::IsDir() const noexcept
    {
        return AreValuesClose(w, static_cast<T>(0));
    }

    template<Numeric T>
    constexpr bool Vector4<T>::IsPos() const noexcept
    {
        return w > static_cast<T>(0);
    }
}

export using Vec4  = jpt::Vector4<Precision>;
export using Vec4f = jpt::Vector4<float32>;
export using Vec4d = jpt::Vector4<float64>;
export using Vec4i = jpt::Vector4<int32>;

template<> constexpr bool jpt::IsTrivial<Vec4f> = true;
template<> constexpr bool jpt::IsTrivial<Vec4i> = true;