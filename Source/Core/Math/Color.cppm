// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Color;

import jpt.LinearColor;
import jpt.TypeDefs;
import std;

export namespace jpt
{
    /** Eight bits a channel, R,G,B,A in memory order -- what an image file holds and what a hex
        code means. LinearColor is the type to do arithmetic in; this one is storage and authoring,
        and deliberately has no operators, because eight-bit channels wrap.

        ToLinear and FromLinear rescale between 0..255 and 0..1 and apply no transfer function.
        Whether these bytes are sRGB-encoded belongs to whatever produced them -- for a texture it
        is the pixel format that tells the GPU to decode, and nothing on the CPU ever encodes. */
    struct Color
    {
    public:
        uint8 r = 0;
        uint8 g = 0;
        uint8 b = 0;
        uint8 a = 255;

    public:
        constexpr Color() noexcept = default;
        constexpr Color(uint8 inR, uint8 inG, uint8 inB, uint8 inA = 255) noexcept;

    public:
        /** Packed 0xRRGGBBAA, which is the reverse of the member order in memory on a little-endian
            machine -- hence a named conversion rather than a cast. */
        [[nodiscard]] static constexpr Color FromRGBA(uint32 rgba) noexcept;
        [[nodiscard]] constexpr uint32 ToRGBA() const noexcept;

        [[nodiscard]] static constexpr Color FromLinear(const LinearColor& linear) noexcept;
        [[nodiscard]] constexpr LinearColor ToLinear() const noexcept;

        [[nodiscard]] constexpr bool operator==(const Color& other) const noexcept = default;
    };

    // The whole basis of a texture upload: pixels.data() is bound as tightly packed RGBA8.
    static_assert(sizeof(Color) == 4, "Color must be four bytes with no padding");
    static_assert(std::is_standard_layout_v<Color>, "Color crosses to the GPU as raw bytes");

    // Member functions
    constexpr Color::Color(uint8 inR, uint8 inG, uint8 inB, uint8 inA) noexcept
        : r(inR)
        , g(inG)
        , b(inB)
        , a(inA)
    {
    }

    constexpr Color Color::FromRGBA(uint32 rgba) noexcept
    {
        return Color(static_cast<uint8>((rgba >> 24) & 0xFF),
                     static_cast<uint8>((rgba >> 16) & 0xFF),
                     static_cast<uint8>((rgba >>  8) & 0xFF),
                     static_cast<uint8>( rgba        & 0xFF));
    }

    constexpr uint32 Color::ToRGBA() const noexcept
    {
        return (static_cast<uint32>(r) << 24) |
               (static_cast<uint32>(g) << 16) |
               (static_cast<uint32>(b) <<  8) |
                static_cast<uint32>(a);
    }

    constexpr Color Color::FromLinear(const LinearColor& linear) noexcept
    {
        // Clamped before the cast because an out-of-range float-to-integer conversion is undefined,
        // and rounded rather than truncated, or 0.5 lands on 127 instead of 128.
        constexpr auto quantize = [](float32 value)
        {
            return static_cast<uint8>(std::clamp(value, 0.0f, 1.0f) * 255.0f + 0.5f);
        };

        return Color(quantize(linear.r), quantize(linear.g), quantize(linear.b), quantize(linear.a));
    }

    constexpr LinearColor Color::ToLinear() const noexcept
    {
        constexpr float32 kInv255 = 1.0f / 255.0f;
        return LinearColor(r * kInv255, g * kInv255, b * kInv255, a * kInv255);
    }
}
