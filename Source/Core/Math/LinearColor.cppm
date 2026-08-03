// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.LinearColor;

import jpt.TypeDefs;

export namespace jpt
{
    /** Linear RGBA, each component 0 to 1. Goes to the GPU as-is: the layer's _sRGB pixel
        format applies the display transfer function on write, so nothing here ever encodes. */
    struct LinearColor
    {
    public:
        float32 r = 0.0f;
        float32 g = 0.0f;
        float32 b = 0.0f;
        float32 a = 1.0f;

    public:
        [[nodiscard]] static consteval LinearColor Black()   noexcept { return LinearColor(0.0f, 0.0f, 0.0f); }
        [[nodiscard]] static consteval LinearColor White()   noexcept { return LinearColor(1.0f, 1.0f, 1.0f); }
        [[nodiscard]] static consteval LinearColor Red()     noexcept { return LinearColor(1.0f, 0.0f, 0.0f); }
        [[nodiscard]] static consteval LinearColor Green()   noexcept { return LinearColor(0.0f, 1.0f, 0.0f); }
        [[nodiscard]] static consteval LinearColor Blue()    noexcept { return LinearColor(0.0f, 0.0f, 1.0f); }
        [[nodiscard]] static consteval LinearColor Yellow()  noexcept { return LinearColor(1.0f, 1.0f, 0.0f); }
        [[nodiscard]] static consteval LinearColor Magenta() noexcept { return LinearColor(1.0f, 0.0f, 1.0f); }
        [[nodiscard]] static consteval LinearColor Cyan()    noexcept { return LinearColor(0.0f, 1.0f, 1.0f); }
        [[nodiscard]] static consteval LinearColor Gray() noexcept { return LinearColor(0.5f, 0.5f, 0.5f); }
        [[nodiscard]] static consteval LinearColor Transparent() noexcept { return LinearColor(0.0f, 0.0f, 0.0f, 0.0f); }

    public:
        constexpr LinearColor() noexcept = default;
        constexpr LinearColor(float32 inR, float32 inG, float32 inB, float32 inA = 1.0f) noexcept;

    public:
        [[nodiscard]] constexpr LinearColor operator+(const LinearColor& other) const noexcept;
        [[nodiscard]] constexpr LinearColor operator-(const LinearColor& other) const noexcept;
        [[nodiscard]] constexpr LinearColor operator*(float32 scalar) const noexcept;
        [[nodiscard]] constexpr LinearColor operator/(float32 scalar) const noexcept;

        constexpr LinearColor& operator+=(const LinearColor& other) noexcept;
        constexpr LinearColor& operator-=(const LinearColor& other) noexcept;
        constexpr LinearColor& operator*=(float32 scalar) noexcept;
        constexpr LinearColor& operator/=(float32 scalar) noexcept;

        [[nodiscard]] constexpr bool operator==(const LinearColor& other) const noexcept = default;
    };

    // ------------------------------------------------------------------------------------------------
    // Non-Member functions
    // ------------------------------------------------------------------------------------------------
    [[nodiscard]] constexpr LinearColor operator*(float32 scalar, const LinearColor& color) noexcept
    {
        return color * scalar;
    }

    // ------------------------------------------------------------------------------------------------
    // Member functions
    // ------------------------------------------------------------------------------------------------
    constexpr LinearColor::LinearColor(float32 inR, float32 inG, float32 inB, float32 inA) noexcept
        : r(inR)
        , g(inG)
        , b(inB)
        , a(inA)
    {
    }

    constexpr LinearColor LinearColor::operator+(const LinearColor& other) const noexcept
    {
        return LinearColor(r + other.r, g + other.g, b + other.b, a + other.a);
    }

    constexpr LinearColor LinearColor::operator-(const LinearColor& other) const noexcept
    {
        return LinearColor(r - other.r, g - other.g, b - other.b, a - other.a);
    }

    constexpr LinearColor LinearColor::operator*(float32 scalar) const noexcept
    {
        return LinearColor(r * scalar, g * scalar, b * scalar, a * scalar);
    }

    constexpr LinearColor LinearColor::operator/(float32 scalar) const noexcept
    {
        return LinearColor(r / scalar, g / scalar, b / scalar, a / scalar);
    }

    constexpr LinearColor& LinearColor::operator+=(const LinearColor& other) noexcept
    {
        r += other.r;
        g += other.g;
        b += other.b;
        a += other.a;
        return *this;
    }

    constexpr LinearColor& LinearColor::operator-=(const LinearColor& other) noexcept
    {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        a -= other.a;
        return *this;
    }

    constexpr LinearColor& LinearColor::operator*=(float32 scalar) noexcept
    {
        r *= scalar;
        g *= scalar;
        b *= scalar;
        a *= scalar;
        return *this;
    }

    constexpr LinearColor& LinearColor::operator/=(float32 scalar) noexcept
    {
        r /= scalar;
        g /= scalar;
        b /= scalar;
        a /= scalar;
        return *this;
    }
}
