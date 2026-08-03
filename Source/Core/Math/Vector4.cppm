// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Vector4;

import jpt.Concepts;
import jpt.Constants;
import jpt.Math;
import jpt.TypeDefs;
import jpt.Vector3;

export namespace jpt
{
    template<Numeric T>
    struct Vector4
    {
        T x = static_cast<T>(0);
        T y = static_cast<T>(0);
        T z = static_cast<T>(0);
        T w = static_cast<T>(0);

        constexpr Vector4() = default;

        /** Broadcasts to all four. Legacy set w to 1 here, so the implicit Vector4(0.0f) built
            (0,0,0,1) instead of zero -- inconsistent with Vector2/Vector3 and easy to miss. */
        constexpr Vector4(T scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}

        constexpr Vector4(T inX, T inY, T inZ, T inW) : x(inX), y(inY), z(inZ), w(inW) {}
        constexpr Vector4(const Vector3<T>& v, T inW)  : x(v.x), y(v.y), z(v.z), w(inW) {}

        static consteval Vector4 Zero() { return Vector4(static_cast<T>(0)); }
        static consteval Vector4 One()  { return Vector4(static_cast<T>(1)); }

        [[nodiscard]] constexpr Vector3<T> XYZ() const { return Vector3<T>(x, y, z); }

        /** A switch rather than legacy's (&x)[index]: pointer arithmetic past the first member
            is undefined, and is rejected outright during constant evaluation. */
        constexpr T& operator[](usize index)
        {
            switch (index) { case 0: return x; case 1: return y; case 2: return z; default: return w; }
        }
        constexpr const T& operator[](usize index) const
        {
            switch (index) { case 0: return x; case 1: return y; case 2: return z; default: return w; }
        }

        constexpr Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }

        constexpr Vector4 operator+(const Vector4& o) const { return Vector4(x + o.x, y + o.y, z + o.z, w + o.w); }
        constexpr Vector4 operator-(const Vector4& o) const { return Vector4(x - o.x, y - o.y, z - o.z, w - o.w); }
        constexpr Vector4 operator*(T scalar)         const { return Vector4(x * scalar, y * scalar, z * scalar, w * scalar); }
        constexpr Vector4 operator/(T scalar)         const { return Vector4(x / scalar, y / scalar, z / scalar, w / scalar); }

        constexpr Vector4& operator+=(const Vector4& o) { x += o.x; y += o.y; z += o.z; w += o.w; return *this; }
        constexpr Vector4& operator-=(const Vector4& o) { x -= o.x; y -= o.y; z -= o.z; w -= o.w; return *this; }
        constexpr Vector4& operator*=(T scalar)         { x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this; }
        constexpr Vector4& operator/=(T scalar)         { x /= scalar; y /= scalar; z /= scalar; w /= scalar; return *this; }

        constexpr bool operator==(const Vector4& o) const = default;

        /** All four components, unlike legacy's Length()/Dot() which silently dropped w. */
        [[nodiscard]] constexpr T Dot(const Vector4& o) const { return x * o.x + y * o.y + z * o.z + w * o.w; }
        [[nodiscard]] constexpr T Length2() const { return Dot(*this); }

        [[nodiscard]] T Length() const requires Floating<T> { return Sqrt(Length2()); }
    };

    template<Numeric T>
    constexpr Vector4<T> operator*(T scalar, const Vector4<T>& v) { return v * scalar; }

    using Vec4  = Vector4<float32>;
    using Vec4d = Vector4<float64>;
    using Vec4i = Vector4<int32>;
}
