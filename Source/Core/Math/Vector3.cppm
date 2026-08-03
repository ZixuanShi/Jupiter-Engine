// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Vector3;

import jpt.Concepts;
import jpt.Constants;
import jpt.Math;
import jpt.TypeDefs;

export namespace jpt
{
    template<Numeric T>
    struct Vector3
    {
        T x = static_cast<T>(0);
        T y = static_cast<T>(0);
        T z = static_cast<T>(0);

        constexpr Vector3() = default;
        constexpr Vector3(T scalar) : x(scalar), y(scalar), z(scalar) {}
        constexpr Vector3(T inX, T inY, T inZ) : x(inX), y(inY), z(inZ) {}

        /** Right-handed, Y up, forward -Z -- the glTF 2.0 and Maya convention, so meshes import
            with no axis conversion. Legacy declared forward as +Z while its LookAt and rotations
            were right-handed; the contradiction forced call sites to negate their right axis. */
        static consteval Vector3 Right()    { return Vector3(static_cast<T>( 1), static_cast<T>( 0), static_cast<T>( 0)); }
        static consteval Vector3 Left()     { return Vector3(static_cast<T>(-1), static_cast<T>( 0), static_cast<T>( 0)); }
        static consteval Vector3 Up()       { return Vector3(static_cast<T>( 0), static_cast<T>( 1), static_cast<T>( 0)); }
        static consteval Vector3 Down()     { return Vector3(static_cast<T>( 0), static_cast<T>(-1), static_cast<T>( 0)); }
        static consteval Vector3 Forward()  { return Vector3(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>(-1)); }
        static consteval Vector3 Backward() { return Vector3(static_cast<T>( 0), static_cast<T>( 0), static_cast<T>( 1)); }
        static consteval Vector3 Zero()     { return Vector3(static_cast<T>( 0)); }
        static consteval Vector3 One()      { return Vector3(static_cast<T>( 1)); }

        constexpr Vector3 operator-() const { return Vector3(-x, -y, -z); }

        constexpr Vector3 operator+(const Vector3& o) const { return Vector3(x + o.x, y + o.y, z + o.z); }
        constexpr Vector3 operator-(const Vector3& o) const { return Vector3(x - o.x, y - o.y, z - o.z); }
        constexpr Vector3 operator*(T scalar)         const { return Vector3(x * scalar, y * scalar, z * scalar); }
        constexpr Vector3 operator/(T scalar)         const { return Vector3(x / scalar, y / scalar, z / scalar); }

        constexpr Vector3& operator+=(const Vector3& o) { x += o.x; y += o.y; z += o.z; return *this; }
        constexpr Vector3& operator-=(const Vector3& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
        constexpr Vector3& operator*=(T scalar)         { x *= scalar; y *= scalar; z *= scalar; return *this; }
        constexpr Vector3& operator/=(T scalar)         { x /= scalar; y /= scalar; z /= scalar; return *this; }

        constexpr bool operator==(const Vector3& o) const = default;

        [[nodiscard]] constexpr T Dot(const Vector3& o) const { return x * o.x + y * o.y + z * o.z; }

        /** Right-handed: Right().Cross(Up()) == Backward(), so the basis follows x cross y = z. */
        [[nodiscard]] constexpr Vector3 Cross(const Vector3& o) const
        {
            return Vector3(y * o.z - z * o.y,
                           z * o.x - x * o.z,
                           x * o.y - y * o.x);
        }

        [[nodiscard]] constexpr T Length2() const { return Dot(*this); }

        [[nodiscard]] T Length() const requires Floating<T> { return Sqrt(Length2()); }

        [[nodiscard]] T Distance(const Vector3& o) const requires Floating<T> { return (*this - o).Length(); }

        /** Guards against kEpsilon rather than exact zero: a denormal length divides to inf. */
        void Normalize() requires Floating<T>
        {
            if (const T length = Length(); length > kEpsilon<T>)
            {
                *this /= length;
            }
        }

        [[nodiscard]] Vector3 Normalized() const requires Floating<T>
        {
            Vector3 result = *this;
            result.Normalize();
            return result;
        }
    };

    template<Numeric T>
    constexpr Vector3<T> operator*(T scalar, const Vector3<T>& v) { return v * scalar; }

    using Vec3  = Vector3<float32>;
    using Vec3d = Vector3<float64>;
    using Vec3i = Vector3<int32>;
}
