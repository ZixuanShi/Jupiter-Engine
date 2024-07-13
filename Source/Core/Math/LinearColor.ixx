// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.LinearColor;

import jpt.Math;
import jpt.String;
import jpt.TypeDefs;
import jpt.TypeTraits;

namespace jpt
{
	/** 16 bytes floating-point RGBA color. Each component ranged [0.0f - 1.0f] */
	export struct LinearColor
	{
#pragma region Presets
	public:
		static const LinearColor Black;
		static const LinearColor White;
		static const LinearColor Red;
		static const LinearColor Green;
		static const LinearColor Blue;
		static const LinearColor Yellow;
		static const LinearColor Magenta;
		static const LinearColor Cyan;
		static const LinearColor Gray;
		static const LinearColor LightGray;
		static const LinearColor DarkGray;
		static const LinearColor LightRed;
		static const LinearColor LightGreen;
		static const LinearColor LightBlue;
		static const LinearColor LightYellow;
		static const LinearColor LightMagenta;
		static const LinearColor LightCyan;
		static const LinearColor DarkRed;
		static const LinearColor DarkGreen;
		static const LinearColor DarkBlue;
		static const LinearColor DarkYellow;
		static const LinearColor DarkMagenta;
		static const LinearColor DarkCyan;
		static const LinearColor Orange;
		static const LinearColor Brown;
		static const LinearColor Pink;
		static const LinearColor Purple;
		static const LinearColor Violet;
		static const LinearColor Gold;
		static const LinearColor Silver;
		static const LinearColor Bronze;
		static const LinearColor Transparent;
#pragma endregion Presets

	public:
		float32 r = 0.0f;
		float32 g = 0.0f;
		float32 b = 0.0f;
		float32 a = 1.0f;

	public:
		constexpr LinearColor() noexcept = default;
		constexpr LinearColor(uint32 rgba) noexcept;
		constexpr LinearColor(float32 r, float32 g, float32 b, float32 a = 1.0f) noexcept;

		constexpr LinearColor operator+(LinearColor other) const noexcept;
		constexpr LinearColor operator-(LinearColor other) const noexcept;
		constexpr LinearColor operator*(float32 scalar) const noexcept;
		constexpr LinearColor operator/(float32 scalar) const noexcept;

		constexpr LinearColor& operator+=(LinearColor other) noexcept;
		constexpr LinearColor& operator-=(LinearColor other) noexcept;
		constexpr LinearColor& operator*=(float32 scalar) noexcept;
		constexpr LinearColor& operator/=(float32 scalar) noexcept;

		constexpr void FromRGBA(uint32 rgba) noexcept;
		constexpr uint32 ToRGBA() const noexcept;
		constexpr String ToString() const noexcept;

		constexpr bool operator==(LinearColor other) const noexcept;
	};

	// ------------------------------------------------------------------------------------------------
	// Non-member functions
	// ------------------------------------------------------------------------------------------------
	export constexpr LinearColor operator*(float32 scalar, LinearColor color) noexcept
	{
		return color * scalar;
	}

	export constexpr LinearColor operator/(float32 scalar, LinearColor color) noexcept
	{
		return color / scalar;
	}

	// ------------------------------------------------------------------------------------------------
	// Member functions
	// ------------------------------------------------------------------------------------------------
	constexpr LinearColor::LinearColor(uint32 rgba) noexcept
	{
		FromRGBA(rgba);
	}

	constexpr LinearColor::LinearColor(float32 r, float32 g, float32 b, float32 a) noexcept
		: r(r)
		, g(g)
		, b(b)
		, a(a)
	{
	}

	constexpr LinearColor LinearColor::operator+(LinearColor other) const noexcept
	{
		return LinearColor(r + other.r, 
			g + other.g, 
			b + other.b, 
			a + other.a);
	}

	constexpr LinearColor LinearColor::operator-(LinearColor other) const noexcept
	{
		return LinearColor(r - other.r,
			g - other.g,
			b - other.b,
			a - other.a);
	}

	constexpr LinearColor LinearColor::operator*(float32 scalar) const noexcept
	{
		return LinearColor(r * scalar,
			g * scalar,
			b * scalar,
			a * scalar);
	}

	constexpr LinearColor LinearColor::operator/(float32 scalar) const noexcept
	{
		const float32 inv = 1.0f / scalar;
		return LinearColor(r * inv,
			g * inv,
			b * inv,
			a * inv);
	}

	constexpr LinearColor& LinearColor::operator+=(LinearColor other) noexcept
	{
		*this = *this + other;
		return *this;
	}

	constexpr LinearColor& LinearColor::operator-=(LinearColor other) noexcept
	{
		*this = *this - other;
		return *this;
	}

	constexpr LinearColor& LinearColor::operator*=(float32 scalar) noexcept
	{
		*this = *this * scalar;
		return *this;
	}

	constexpr LinearColor& LinearColor::operator/=(float32 scalar) noexcept
	{
		*this = *this / scalar;
		return *this;
	}

	constexpr void LinearColor::FromRGBA(uint32 rgba) noexcept
	{
		const float32 inv = 1.0f / 255.0f;

		const uint8 _r = (rgba >> 24) & 0xFF;
		const uint8 _g = (rgba >> 16) & 0xFF;
		const uint8 _b = (rgba >> 8) & 0xFF;
		const uint8 _a = rgba & 0xFF;

		r = static_cast<float32>(_r) * inv;
		g = static_cast<float32>(_g) * inv;
		b = static_cast<float32>(_b) * inv;
		a = static_cast<float32>(_a) * inv;
	}

	constexpr uint32 LinearColor::ToRGBA() const noexcept
	{
		uint32 rgba = 0;

		const uint8 _r = static_cast<uint8>(r * 255.0f);
		const uint8 _g = static_cast<uint8>(g * 255.0f);
		const uint8 _b = static_cast<uint8>(b * 255.0f);
		const uint8 _a = static_cast<uint8>(a * 255.0f);

		rgba |= (_r << 24);
		rgba |= (_g << 16);
		rgba |= (_b << 8);
		rgba |= _a;
		return rgba;
	}

	constexpr String LinearColor::ToString() const noexcept
	{
		return String::Format<32>("r: %f, g: %f, b: %f, a: %f", r, g, b, a);
	}

	constexpr bool LinearColor::operator==(LinearColor other) const noexcept
	{
		return AreValuesClose(r, other.r) &&
			   AreValuesClose(g, other.g) &&
			   AreValuesClose(b, other.b) &&
			   AreValuesClose(a, other.a);
	}
}

export using LinearColor = jpt::LinearColor;
template<> constexpr bool jpt::IsTrivial<LinearColor> = true;

#pragma region Presets
const LinearColor LinearColor::Black        = {  0.0f,  0.0f,  0.0f };
const LinearColor LinearColor::White        = {  1.0f,  1.0f,  1.0f };
const LinearColor LinearColor::Red          = {  1.0f,  0.0f,  0.0f };
const LinearColor LinearColor::Green        = {  0.0f,  1.0f,  0.0f };
const LinearColor LinearColor::Blue         = {  0.0f,  0.0f,  1.0f };
const LinearColor LinearColor::Yellow       = {  1.0f,  1.0f,  0.0f };
const LinearColor LinearColor::Magenta      = {  1.0f,  0.0f,  1.0f };
const LinearColor LinearColor::Cyan         = {  0.0f,  1.0f,  1.0f };
const LinearColor LinearColor::Gray         = {  0.5f,  0.5f,  0.5f };
const LinearColor LinearColor::LightGray    = { 0.75f, 0.75f, 0.75f };
const LinearColor LinearColor::DarkGray     = { 0.25f, 0.25f, 0.25f };
const LinearColor LinearColor::LightRed     = {  1.0f,  0.5f,  0.5f };
const LinearColor LinearColor::LightGreen   = {  0.5f,  1.0f,  0.5f };
const LinearColor LinearColor::LightBlue    = {  0.5f,  0.5f,  1.0f };
const LinearColor LinearColor::LightYellow  = {  1.0f,  1.0f,  0.5f };
const LinearColor LinearColor::LightMagenta = {  1.0f,  0.5f,  1.0f };
const LinearColor LinearColor::LightCyan    = {  0.5f,  1.0f,  1.0f };
const LinearColor LinearColor::DarkRed      = {  0.5f,  0.0f,  0.0f };
const LinearColor LinearColor::DarkGreen    = {  0.0f,  0.5f,  0.0f };
const LinearColor LinearColor::DarkBlue     = {  0.0f,  0.0f,  0.5f };
const LinearColor LinearColor::DarkYellow   = {  0.5f,  0.5f,  0.0f };
const LinearColor LinearColor::DarkMagenta  = {  0.5f,  0.0f,  0.5f };
const LinearColor LinearColor::DarkCyan     = {  0.0f,  0.5f,  0.5f };
const LinearColor LinearColor::Orange       = {  1.0f,  0.5f,  0.0f };
const LinearColor LinearColor::Brown        = {  0.6f,  0.4f,  0.2f };
const LinearColor LinearColor::Pink         = {  1.0f, 0.75f,  0.8f };
const LinearColor LinearColor::Purple       = {  0.5f,  0.0f,  0.5f };
const LinearColor LinearColor::Violet       = {  0.5f,  0.0f,  1.0f };
const LinearColor LinearColor::Gold         = {  1.0f, 0.84f,  0.0f };
const LinearColor LinearColor::Silver       = { 0.75f, 0.75f, 0.75f };
const LinearColor LinearColor::Bronze       = {  0.8f,  0.5f,  0.2f };
const LinearColor LinearColor::Transparent  = {  0.0f,  0.0f,  0.0f,  0.0f };
#pragma endregion Presets