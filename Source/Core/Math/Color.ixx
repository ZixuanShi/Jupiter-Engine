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
		static consteval Color Black()        { return {   0,   0,   0 }; }
		static consteval Color White() 		  { return { 255, 255, 255 }; }
		static consteval Color Red() 		  { return { 255,   0,   0 }; }
		static consteval Color Green() 		  { return {   0, 255,   0 }; }
		static consteval Color Blue() 		  { return {   0,   0, 255 }; }
		static consteval Color Yellow() 	  { return { 255, 255,   0 }; }
		static consteval Color Magenta() 	  { return { 255,   0, 255 }; }
		static consteval Color Cyan() 		  { return {   0, 255, 255 }; }
		static consteval Color Gray() 		  { return { 128, 128, 128 }; }
		static consteval Color LightGray() 	  { return { 192, 192, 192 }; }
		static consteval Color DarkGray() 	  { return {  64,  64,  64 }; }
		static consteval Color LightRed() 	  { return { 255, 128, 128 }; }
		static consteval Color LightGreen()   { return { 128, 255, 128 }; }
		static consteval Color LightBlue() 	  { return { 128, 128, 255 }; }
		static consteval Color LightYellow()  { return { 255, 255, 128 }; }
		static consteval Color LightMagenta() { return { 255, 128, 255 }; }
		static consteval Color LightCyan() 	  { return { 128, 255, 255 }; }
		static consteval Color DarkRed() 	  { return { 128,   0,   0 }; }
		static consteval Color DarkGreen() 	  { return {   0, 128,   0 }; }
		static consteval Color DarkBlue() 	  { return {   0,   0, 128 }; }
		static consteval Color DarkYellow()   { return { 128, 128,   0 }; }
		static consteval Color DarkMagenta()  { return { 128,   0, 128 }; }
		static consteval Color DarkCyan() 	  { return {   0, 128, 128 }; }
		static consteval Color Orange() 	  { return { 255, 165,   0 }; }
		static consteval Color Brown() 		  { return { 165,  42,  42 }; }
		static consteval Color Pink() 		  { return { 255, 192, 203 }; }
		static consteval Color Purple() 	  { return { 128,   0, 128 }; }
		static consteval Color Violet() 	  { return { 238, 130, 238 }; }
		static consteval Color Gold() 		  { return { 255, 215,   0 }; }
		static consteval Color Silver() 	  { return { 192, 192, 192 }; }
		static consteval Color Bronze() 	  { return { 205, 127,  50 }; }
		static consteval Color Transparent()  { return { 0,   0,   0,   0 }; }
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