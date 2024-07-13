// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Color;

import jpt.LinearColor;
import jpt.Math;
import jpt.String;
import jpt.TypeDefs;
import jpt.TypeTraits;

namespace jpt
{
	/** 32-bit RGBA integer */
	export struct Color
	{
#pragma region Presets
	public:
		static const Color Black;
		static const Color White;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		static const Color Magenta;
		static const Color Cyan;
		static const Color Gray;
		static const Color LightGray;
		static const Color DarkGray;
		static const Color LightRed;
		static const Color LightGreen;
		static const Color LightBlue;
		static const Color LightYellow;
		static const Color LightMagenta;
		static const Color LightCyan;
		static const Color DarkRed;
		static const Color DarkGreen;
		static const Color DarkBlue;
		static const Color DarkYellow;
		static const Color DarkMagenta;
		static const Color DarkCyan;
		static const Color Orange;
		static const Color Brown;
		static const Color Pink;
		static const Color Purple;
		static const Color Violet;
		static const Color Gold;
		static const Color Silver;
		static const Color Bronze;
		static const Color Transparent;
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

		constexpr Color operator+(Color other) const noexcept;
		constexpr Color operator-(Color other) const noexcept;
		constexpr Color operator*(float32 scalar) const noexcept;
		constexpr Color operator/(float32 scalar) const noexcept;

		constexpr Color& operator+=(Color other) noexcept;
		constexpr Color& operator-=(Color other) noexcept;
		constexpr Color& operator*=(float32 scalar) noexcept;
		constexpr Color& operator/=(float32 scalar) noexcept;

		constexpr void FromRGBA(uint32 rgba) noexcept;
		constexpr void FromLinearColor(LinearColor linearColor) noexcept;

		constexpr uint32 ToRGBA() const noexcept;
		constexpr String ToString() const noexcept;

		constexpr bool operator==(Color other) const noexcept;
	};

	// ------------------------------------------------------------------------------------------------
	// Non-member functions
	// ------------------------------------------------------------------------------------------------
	export constexpr Color operator*(float32 scalar, Color color) noexcept
	{
		return color * scalar;
	}

	export constexpr Color operator/(float32 scalar, Color color) noexcept
	{
		return color / scalar;
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

	constexpr String Color::ToString() const noexcept
	{
		return String::Format<32>("r: %u, g: %u, b: %u, a: %u", r, g, b, a);
	}

	constexpr bool Color::operator==(Color other) const noexcept
	{
		return r == other.r &&
			   g == other.g &&
			   b == other.b &&
			   a == other.a;
	}
}

export using Color = jpt::Color;
template<> constexpr bool jpt::IsTrivial<Color> = true;

#pragma region Presets
const Color Color::Black        = {   0,   0,   0 };
const Color Color::White        = { 255, 255, 255 };
const Color Color::Red          = { 255,   0,   0 };
const Color Color::Green        = {   0, 255,   0 };
const Color Color::Blue         = {   0,   0, 255 };
const Color Color::Yellow       = { 255, 255,   0 };
const Color Color::Magenta      = { 255,   0, 255 };
const Color Color::Cyan         = {   0, 255, 255 };
const Color Color::Gray         = { 128, 128, 128 };
const Color Color::LightGray    = { 192, 192, 192 };
const Color Color::DarkGray     = {  64,  64,  64 };
const Color Color::LightRed     = { 255, 128, 128 };
const Color Color::LightGreen   = { 128, 255, 128 };
const Color Color::LightBlue    = { 128, 128, 255 };
const Color Color::LightYellow  = { 255, 255, 128 };
const Color Color::LightMagenta = { 255, 128, 255 };
const Color Color::LightCyan    = { 128, 255, 255 };
const Color Color::DarkRed      = { 128,   0,   0 };
const Color Color::DarkGreen    = {   0, 128,   0 };
const Color Color::DarkBlue     = {   0,   0, 128 };
const Color Color::DarkYellow   = { 128, 128,   0 };
const Color Color::DarkMagenta  = { 128,   0, 128 };
const Color Color::DarkCyan     = {   0, 128, 128 };
const Color Color::Orange       = { 255, 165,   0 };
const Color Color::Brown        = { 165,  42,  42 };
const Color Color::Pink         = { 255, 192, 203 };
const Color Color::Purple       = { 128,   0, 128 };
const Color Color::Violet       = { 238, 130, 238 };
const Color Color::Gold         = { 255, 215,   0 };
const Color Color::Silver       = { 192, 192, 192 };
const Color Color::Bronze       = { 205, 127,  50 };
const Color Color::Transparent  = { 0,   0,   0,   0 };
#pragma endregion Presets