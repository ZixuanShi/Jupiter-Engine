// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <type_traits>

export module jpt.Vector3;

import jpt.Constants;
import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Math;
import jpt.String;
import jpt.Hash;
import jpt.Vector2;

export namespace jpt
{
    template<Numeric T>
    struct Vector3
    {
    public:
        T x = static_cast<T>(0); /**< i, right */
        T y = static_cast<T>(0); /**< j, up */
        T z = static_cast<T>(0); /**< k, forward */

    public:
        using NumericType = T;

        [[nodiscard]] static consteval Vector3 Zero() noexcept { return Vector3( static_cast<T>(0),  static_cast<T>(0),  static_cast<T>(0)); }
        [[nodiscard]] static consteval Vector3 One()  noexcept { return Vector3( static_cast<T>(1),  static_cast<T>(1),  static_cast<T>(1)); }

        // Right-handed. X = right (pitch), Y = up (yaw), Z = forward (roll).
        [[nodiscard]] static consteval Vector3 Right()    noexcept { return Vector3(static_cast<T>( 1), static_cast<T>( 0), static_cast<T>( 0)); }
        [[nodiscard]] static consteval Vector3 Left()     noexcept { return Vector3(static_cast<T>(-1), static_cast<T>( 0), static_cast<T>( 0)); }
        [[nodiscard]] static consteval Vector3 Up()       noexcept { return Vector3(static_cast<T>( 0), static_cast<T>( 1), static_cast<T>( 0)); }
        [[nodiscard]] static consteval Vector3 Down()     noexcept { return Vector3(static_cast<T>( 0), static_cast<T>(-1), static_cast<T>( 0)); }
        [[nodiscard]] static consteval Vector3 Forward()  noexcept { return Vector3(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>( 1)); }
        [[nodiscard]] static consteval Vector3 Backward() noexcept { return Vector3(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>(-1)); }

    public:
        constexpr Vector3() = default;
        constexpr Vector3(T scalar);
        constexpr Vector3(T _x, T _y, T _z);
        constexpr Vector3(const Vector2<T>& vector2, T _z);

    public:
        [[nodiscard]] constexpr Vector3 operator-() const noexcept;

        [[nodiscard]] constexpr Vector3 operator+(const Vector3& other) const noexcept;
        [[nodiscard]] constexpr Vector3 operator-(const Vector3& other) const noexcept;
        [[nodiscard]] constexpr Vector3 operator*(const Vector3& other) const noexcept;
        [[nodiscard]] constexpr Vector3 operator/(const Vector3& other) const noexcept;

        constexpr Vector3& operator+=(const Vector3& other) noexcept;
        constexpr Vector3& operator-=(const Vector3& other) noexcept;
        constexpr Vector3& operator*=(const Vector3& other) noexcept;
        constexpr Vector3& operator/=(const Vector3& other) noexcept;

        [[nodiscard]] constexpr Vector3 operator+(T scalar) const noexcept;
        [[nodiscard]] constexpr Vector3 operator-(T scalar) const noexcept;
        [[nodiscard]] constexpr Vector3 operator*(T scalar) const noexcept;
        [[nodiscard]] constexpr Vector3 operator/(T scalar) const noexcept;

        constexpr Vector3& operator+=(T scalar) noexcept;
        constexpr Vector3& operator-=(T scalar) noexcept;
        constexpr Vector3& operator*=(T scalar) noexcept;
        constexpr Vector3& operator/=(T scalar) noexcept;

        [[nodiscard]] constexpr T& operator[](size_t index);
        [[nodiscard]] constexpr const T& operator[](size_t index) const;

    public:
        [[nodiscard]] constexpr T Dot(const Vector3& other) const noexcept;
        [[nodiscard]] constexpr Vector3 Cross(const Vector3& other) const noexcept;

        [[nodiscard]] constexpr T Length() const noexcept;
        [[nodiscard]] constexpr T Length2() const noexcept;  // 2 means squared

        [[nodiscard]] constexpr T Distance(const Vector3& other) const noexcept;
        [[nodiscard]] constexpr T Distance2(const Vector3& other) const noexcept;  // 2 means squared

        constexpr void Normalize() noexcept;
        [[nodiscard]] constexpr Vector3 Normalized() const noexcept;

        constexpr bool IsOnLeft(const Vector3& viewPosition, const Vector3& viewDirection) const;

        constexpr static T Angle(const Vector3& from, const Vector3& to); // Unsigned, faster (no atan2)
        constexpr static T AngleSigned(const Vector3& from, const Vector3& to, const Vector3& axis);  // Signed, but slower

        constexpr static Vector3 Project(const Vector3& from, const Vector3& to);
    };

    // ------------------------------------------------------------------------------------------------
    // Non-Member functions
    // ------------------------------------------------------------------------------------------------
    template<Numeric T>
    [[nodiscard]] constexpr bool operator==(const Vector3<T>& lhs, const Vector3<T>& rhs) noexcept
    {
        return AreValuesClose(lhs.x, rhs.x) &&
               AreValuesClose(lhs.y, rhs.y) &&
               AreValuesClose(lhs.z, rhs.z);
    }
    
    template<Numeric T>
    [[nodiscard]] constexpr Vector3<T> operator+(T scaler, const Vector3<T>& TVector3)
    {
        return TVector3 + scaler;
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector3<T> operator-(T scaler, const Vector3<T>& TVector3)
    {
        return TVector3 - scaler;
    }
    
    template<Numeric T>
    [[nodiscard]] constexpr Vector3<T> operator*(T scaler, const Vector3<T>& TVector3)
    {
        return TVector3 * scaler;
    }
    
    template<Numeric T>
    [[nodiscard]] constexpr Vector3<T> operator/(T scaler, const Vector3<T>& TVector3)
    {
        return TVector3 / scaler;
    }

    template<Numeric T>
    [[nodiscard]] constexpr uint64 Hash(const Vector3<T>& vector3)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            const T epsilon = static_cast<T>(1e-6);
            auto round = [epsilon](T value) -> T
                {
                    return (value < epsilon && value > -epsilon) ? static_cast<T>(0) : value;
                };

            uint64 hash = jpt::Hash(round(vector3.x));
            hash ^= jpt::Hash(round(vector3.y)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= jpt::Hash(round(vector3.z)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
        else
        {
            uint64 hash = jpt::Hash(vector3.x);
            hash ^= jpt::Hash(vector3.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= jpt::Hash(vector3.z) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
    }
        
    template<Numeric T>
    [[nodiscard]] constexpr String ToString(const Vector3<T>& vector3)
    {
        return String::Format<64>("x: %.3f, y: %.3f, z: %.3f", vector3.x, vector3.y, vector3.z);
    }

    // ------------------------------------------------------------------------------------------------
    // Member functions
    // ------------------------------------------------------------------------------------------------
    template<Numeric T>
    constexpr Vector3<T>::Vector3(T scalar)
        : x(scalar)
        , y(scalar)
        , z(scalar)
    {
    }

    template<Numeric T>
    constexpr Vector3<T>::Vector3(T _x, T _y, T _z)
        : x(_x)
        , y(_y)
        , z(_z)
    {
    }

    template<Numeric T>
    constexpr Vector3<T>::Vector3(const Vector2<T>& vector2, T _z)
        : x(vector2.x)
        , y(vector2.y)
        , z(_z)
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
    constexpr Vector3<T> Vector3<T>::operator*(const Vector3& other) const noexcept
    {
        return Vector3(x * other.x, y * other.y, z * other.z);
    }

    template<Numeric T>
    constexpr Vector3<T> Vector3<T>::operator/(const Vector3& other) const noexcept
    {
        return Vector3(x / other.x, y / other.y, z / other.z);
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
    constexpr Vector3<T>& Vector3<T>::operator*=(const Vector3& other) noexcept
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    template<Numeric T>
    constexpr Vector3<T>& Vector3<T>::operator/=(const Vector3& other) noexcept
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    template<Numeric T>
    constexpr Vector3<T> Vector3<T>::operator+(T scalar) const noexcept
    {
        return Vector3(x + scalar, y + scalar, z + scalar);
    }

    template<Numeric T>
    constexpr Vector3<T> Vector3<T>::operator-(T scalar) const noexcept
    {
        return Vector3(x - scalar, y - scalar, z - scalar);
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
    constexpr Vector3<T>& Vector3<T>::operator+=(T scalar) noexcept
    {
        x += scalar;
        y += scalar;
        z += scalar;
        return *this;
    }

    template<Numeric T>
    constexpr Vector3<T>& Vector3<T>::operator-=(T scalar) noexcept
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
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
    constexpr T& Vector3<T>::operator[](size_t index)
    {
        return (&x)[index];
    }

    template<Numeric T>
    constexpr const T& Vector3<T>::operator[](size_t index) const
    {
        return (&x)[index];
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
    constexpr T Vector3<T>::Length() const noexcept
    {
        return Sqrt(Length2());
    }

    template<Numeric T>
    constexpr T Vector3<T>::Length2() const noexcept
    {
        return x * x + y * y + z * z;
    }

    template<Numeric T>
    constexpr T Vector3<T>::Distance(const Vector3& other) const noexcept
    {
        return Sqrt(Distance2(other));
    }

    template<Numeric T>
    constexpr T Vector3<T>::Distance2(const Vector3& other) const noexcept
    {
        return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z);
    }

    template<Numeric T>
    constexpr void Vector3<T>::Normalize() noexcept
    {
        const T length = Length();
        if (length > static_cast<T>(0))
        {
            x /= length;
            y /= length;
            z /= length;
        }
    }

    template<Numeric T>
    constexpr Vector3<T> Vector3<T>::Normalized() const noexcept
    {
        Vector3<T> result = *this;
        result.Normalize();
        return result;
    }

    template<Numeric T>
    constexpr T Vector3<T>::Angle(const Vector3& from, const Vector3& to)
    {
        // Formula: angle = acos(a . b / (|a| * |b|))
        // Assumes from and to are normalized

        const T dot = from.Dot(to);        
        const T angle = Acos(dot);

        return angle;
    }

    template<Numeric T>
    constexpr T Vector3<T>::AngleSigned(const Vector3& from, const Vector3& to, const Vector3& axis)
    {
        // Formula: angle = atan2((a x b) . c, a . b)
        // Assumes from and to are normalized

        const Vector3<T> cross = from.Cross(to);

        const T dot_a_b = from.Dot(to);
        const T dot_cross_Axis = cross.Dot(axis);

        const T angle = Atan2(dot_cross_Axis, dot_a_b);
        return angle;
    }

    template<Numeric T>
    constexpr Vector3<T> Vector3<T>::Project(const Vector3& from, const Vector3& to)
    {
        // Projects from this vector to the other
        // Formula: proj_v(u) = (u . v / |v|^2) * v

        const T length2 = to.Length2();
        if (length2 == static_cast<T>(0))
        {
            return Vector3<T>::Zero();
        }

        const T dot = from.Dot(to);
        const T scalar = dot / length2;
        return to * scalar;
    }

    template<Numeric T>
    constexpr bool Vector3<T>::IsOnLeft(const Vector3& viewPosition, const Vector3& viewDirection) const
    {
        // Calculate the vector from the view position to the point
        const Vector3 viewToPoint = *this - viewPosition;

        // Calculate the cross product of the view direction and the view to point vector
        const Vector3 cross = viewDirection.Cross(viewToPoint);

        // If the cross product is pointing up, the point is to the left of the view
        return cross.y > static_cast<T>(0);
    }
}

export using Vec3  = jpt::Vector3<Precision>;
export using Vec3f = jpt::Vector3<float>;
export using Vec3d = jpt::Vector3<double>;
export using Vec3i = jpt::Vector3<int32>;
export using Vec3u = jpt::Vector3<uint32>;

template<> constexpr bool jpt::IsTrivial<Vec3f> = true;
template<> constexpr bool jpt::IsTrivial<Vec3i> = true;
template<> constexpr bool jpt::IsTrivial<Vec3u> = true;