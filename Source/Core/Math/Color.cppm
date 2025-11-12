// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Color;

import jpt.LinearColor;
import jpt.Math;
import jpt.String;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Hash;

export namespace jpt
{
    /** 32-bit RGBA integer */
    struct Color
    {
#pragma region Presets
    public:
        [[nodiscard]] static consteval Color Black()        noexcept { return {   0,   0,   0 }; }
        [[nodiscard]] static consteval Color White()        noexcept { return { 255, 255, 255 }; }
        [[nodiscard]] static consteval Color Red()          noexcept { return { 255,   0,   0 }; }
        [[nodiscard]] static consteval Color Green()        noexcept { return {   0, 255,   0 }; }
        [[nodiscard]] static consteval Color Blue()         noexcept { return {   0,   0, 255 }; }
        [[nodiscard]] static consteval Color Yellow()       noexcept { return { 255, 255,   0 }; }
        [[nodiscard]] static consteval Color Magenta()      noexcept { return { 255,   0, 255 }; }
        [[nodiscard]] static consteval Color Cyan()         noexcept { return {   0, 255, 255 }; }
        [[nodiscard]] static consteval Color Gray()         noexcept { return { 128, 128, 128 }; }
        [[nodiscard]] static consteval Color LightGray()    noexcept { return { 192, 192, 192 }; }
        [[nodiscard]] static consteval Color DarkGray()     noexcept { return {  64,  64,  64 }; }
        [[nodiscard]] static consteval Color LightRed()     noexcept { return { 255, 128, 128 }; }
        [[nodiscard]] static consteval Color LightGreen()   noexcept { return { 128, 255, 128 }; }
        [[nodiscard]] static consteval Color LightBlue()    noexcept { return { 128, 128, 255 }; }
        [[nodiscard]] static consteval Color LightYellow()  noexcept { return { 255, 255, 128 }; }
        [[nodiscard]] static consteval Color LightMagenta() noexcept { return { 255, 128, 255 }; }
        [[nodiscard]] static consteval Color LightCyan()    noexcept { return { 128, 255, 255 }; }
        [[nodiscard]] static consteval Color DarkRed()      noexcept { return { 128,   0,   0 }; }
        [[nodiscard]] static consteval Color DarkGreen()    noexcept { return {   0, 128,   0 }; }
        [[nodiscard]] static consteval Color DarkBlue()     noexcept { return {   0,   0, 128 }; }
        [[nodiscard]] static consteval Color DarkYellow()   noexcept { return { 128, 128,   0 }; }
        [[nodiscard]] static consteval Color DarkMagenta()  noexcept { return { 128,   0, 128 }; }
        [[nodiscard]] static consteval Color DarkCyan()     noexcept { return {   0, 128, 128 }; }
        [[nodiscard]] static consteval Color Orange()       noexcept { return { 255, 165,   0 }; }
        [[nodiscard]] static consteval Color Brown()        noexcept { return { 165,  42,  42 }; }
        [[nodiscard]] static consteval Color Pink()         noexcept { return { 255, 192, 203 }; }
        [[nodiscard]] static consteval Color Purple()       noexcept { return { 128,   0, 128 }; }
        [[nodiscard]] static consteval Color Violet()       noexcept { return { 238, 130, 238 }; }
        [[nodiscard]] static consteval Color Gold()         noexcept { return { 255, 215,   0 }; }
        [[nodiscard]] static consteval Color Silver()       noexcept { return { 192, 192, 192 }; }
        [[nodiscard]] static consteval Color Bronze()       noexcept { return { 205, 127,  50 }; }
        [[nodiscard]] static consteval Color Transparent()  noexcept { return {   0,   0,   0,   0 }; }
#pragma endregion Presets

    public:
        uint8 r = 0;
        uint8 g = 0;
        uint8 b = 0;
        uint8 a = 255;

    public:
        constexpr Color() noexcept = default;
        constexpr Color(uint32 rgba) noexcept;
        constexpr Color(LinearColor linearColor) noexcept;
        constexpr Color(uint8 r, uint8 g, uint8 b, uint8 a = 255) noexcept;

        [[nodiscard]] constexpr Color operator+(Color other) const noexcept;
        [[nodiscard]] constexpr Color operator-(Color other) const noexcept;
        [[nodiscard]] constexpr Color operator*(float32 scalar) const noexcept;
        [[nodiscard]] constexpr Color operator/(float32 scalar) const noexcept;

        constexpr Color& operator+=(Color other) noexcept;
        constexpr Color& operator-=(Color other) noexcept;
        constexpr Color& operator*=(float32 scalar) noexcept;
        constexpr Color& operator/=(float32 scalar) noexcept;

        constexpr void FromRGBA(uint32 rgba) noexcept;
        constexpr void FromLinearColor(LinearColor linearColor) noexcept;

        [[nodiscard]] constexpr uint32 ToRGBA() const noexcept;
    };

    // ------------------------------------------------------------------------------------------------
    // Non-Member functions
    // ------------------------------------------------------------------------------------------------
    constexpr Color operator*(float32 scalar, Color color) noexcept
    {
        return color * scalar;
    }

    constexpr Color operator/(float32 scalar, Color color) noexcept
    {
        return color / scalar;
    }

    constexpr bool operator==(Color lhs, Color rhs) noexcept
    {
        return lhs.r == rhs.r &&
               lhs.g == rhs.g &&
               lhs.b == rhs.b &&
               lhs.a == rhs.a;
    }

    constexpr uint64 Hash(Color color) noexcept
    {
        uint64 hash = jpt::Hash(color.r);
        hash ^= jpt::Hash(color.g) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= jpt::Hash(color.b) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= jpt::Hash(color.a) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }

    constexpr String ToString(Color color) noexcept
    {
        return String::Format<32>("r: %u, g: %u, b: %u, a: %u", color.r, color.g, color.b, color.a);
    }

    // ------------------------------------------------------------------------------------------------
    // Member functions
    // ------------------------------------------------------------------------------------------------
    constexpr Color::Color(uint32 rgba) noexcept
    {
        FromRGBA(rgba);
    }

    constexpr Color::Color(LinearColor linearColor) noexcept
    {
        FromLinearColor(linearColor);
    }

    constexpr Color::Color(uint8 r, uint8 g, uint8 b, uint8 a /*= 255*/) noexcept
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {
    }

    constexpr Color Color::operator+(Color other) const noexcept
    {
        return Color(r + other.r, 
                     g + other.g, 
                     b + other.b, 
                     a + other.a);
    }

    constexpr Color Color::operator-(Color other) const noexcept
    {
        return Color(r - other.r,
                     g - other.g,
                     b - other.b,
                     a - other.a);
    }

    constexpr Color Color::operator*(float32 scalar) const noexcept
    {
        return Color(static_cast<uint8>(r * scalar),
                     static_cast<uint8>(g * scalar),
                     static_cast<uint8>(b * scalar),
                     static_cast<uint8>(a * scalar));
    }

    constexpr Color Color::operator/(float32 scalar) const noexcept
    {
        return Color(static_cast<uint8>(r / scalar),
                     static_cast<uint8>(g / scalar),
                     static_cast<uint8>(b / scalar),
                     static_cast<uint8>(a / scalar));
    }

    constexpr Color& Color::operator+=(Color other) noexcept
    {
        *this = *this + other;
        return *this;
    }

    constexpr Color& Color::operator-=(Color other) noexcept
    {
        *this = *this - other;
        return *this;
    }

    constexpr Color& Color::operator*=(float32 scalar) noexcept
    {
        *this = *this * scalar;
        return *this;
    }

    constexpr Color& Color::operator/=(float32 scalar) noexcept
    {
        *this = *this / scalar;
        return *this;
    }

    constexpr void Color::FromRGBA(uint32 rgba) noexcept
    {
        r = static_cast<uint8>((rgba >> 24) & 0xFF);
        g = static_cast<uint8>((rgba >> 16) & 0xFF);
        b = static_cast<uint8>((rgba >> 8)  & 0xFF);
        a = static_cast<uint8>(rgba & 0xFF);
    }

    constexpr void Color::FromLinearColor(LinearColor linearColor) noexcept
    {
        r = static_cast<uint8>(linearColor.r * 255.0f);
        g = static_cast<uint8>(linearColor.g * 255.0f);
        b = static_cast<uint8>(linearColor.b * 255.0f);
        a = static_cast<uint8>(linearColor.a * 255.0f);
    }

    constexpr uint32 Color::ToRGBA() const noexcept
    {
        uint32 rgba = 0;

        rgba |= static_cast<uint32>(r) << 24;
        rgba |= static_cast<uint32>(g) << 16;
        rgba |= static_cast<uint32>(b) << 8;
        rgba |= static_cast<uint32>(a);

        return rgba;
    }
}

export using Color = jpt::Color;
template<> constexpr bool jpt::IsTrivial<Color> = true;