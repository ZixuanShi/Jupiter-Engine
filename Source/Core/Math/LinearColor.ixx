// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.LinearColor;

import jpt.Math;
import jpt.String;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Hash;

namespace jpt
{
	/** 16 bytes floating-point RGBA color. Each component ranged [0.0f - 1.0f] */
	export struct LinearColor
	{
#pragma region Presets
	public:
		static consteval LinearColor Black()        { return {  0.0f,  0.0f,  0.0f }; }
		static consteval LinearColor White()	    { return {  1.0f,  1.0f,  1.0f }; }
		static consteval LinearColor Red()		    { return {  1.0f,  0.0f,  0.0f }; }
		static consteval LinearColor Green()	    { return {  0.0f,  1.0f,  0.0f }; }
		static consteval LinearColor Blue()		    { return {  0.0f,  0.0f,  1.0f }; }
		static consteval LinearColor Yellow()	    { return {  1.0f,  1.0f,  0.0f }; }
		static consteval LinearColor Magenta()	    { return {  1.0f,  0.0f,  1.0f }; }
		static consteval LinearColor Cyan()		    { return {  0.0f,  1.0f,  1.0f }; }
		static consteval LinearColor Gray()		    { return {  0.5f,  0.5f,  0.5f }; }
		static consteval LinearColor LightGray()    { return { 0.75f, 0.75f, 0.75f }; }
		static consteval LinearColor DarkGray()	    { return { 0.25f, 0.25f, 0.25f }; }
		static consteval LinearColor LightRed()	    { return {  1.0f,  0.5f,  0.5f }; }
		static consteval LinearColor LightGreen()   { return {  0.5f,  1.0f,  0.5f }; }
		static consteval LinearColor LightBlue()    { return {  0.5f,  0.5f,  1.0f }; }
		static consteval LinearColor LightYellow()  { return {  1.0f,  1.0f,  0.5f }; }
		static consteval LinearColor LightMagenta() { return {  1.0f,  0.5f,  1.0f }; }
		static consteval LinearColor LightCyan()    { return {  0.5f,  1.0f,  1.0f }; }
		static consteval LinearColor DarkRed()	    { return {  0.5f,  0.0f,  0.0f }; }
		static consteval LinearColor DarkGreen()    { return {  0.0f,  0.5f,  0.0f }; }
		static consteval LinearColor DarkBlue()	    { return {  0.0f,  0.0f,  0.5f }; }
		static consteval LinearColor DarkYellow()   { return {  0.5f,  0.5f,  0.0f }; }
		static consteval LinearColor DarkMagenta()  { return {  0.5f,  0.0f,  0.5f }; }
		static consteval LinearColor DarkCyan()	    { return {  0.0f,  0.5f,  0.5f }; }
		static consteval LinearColor Orange()	    { return {  1.0f,  0.5f,  0.0f }; }
		static consteval LinearColor Brown()	    { return {  0.6f,  0.4f,  0.2f }; }
		static consteval LinearColor Pink()		    { return {  1.0f, 0.75f,  0.8f }; }
		static consteval LinearColor Purple()	    { return {  0.5f,  0.0f,  0.5f }; }
		static consteval LinearColor Violet()	    { return {  0.5f,  0.0f,  1.0f }; }
		static consteval LinearColor Gold()		    { return {  1.0f, 0.84f,  0.0f }; }
		static consteval LinearColor Silver()	    { return { 0.75f, 0.75f, 0.75f }; }
		static consteval LinearColor Bronze()	    { return {  0.8f,  0.5f,  0.2f }; }
		static consteval LinearColor Transparent()  { return { 0.0f,  0.0f,  0.0f,  0.0f }; }
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
		constexpr uint64 Hash() const noexcept;
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

	constexpr uint64 LinearColor::Hash() const noexcept
	{
		const float32 epsilon = static_cast<float32>(1e-6);
		auto round = [epsilon](float32 value) -> float32
			{
				return (value < epsilon && value > -epsilon) ? static_cast<float32>(0) : value;
			};

		uint64 hash = jpt::Hash(round(r));
		hash ^= jpt::Hash(round(g)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		hash ^= jpt::Hash(round(b)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		hash ^= jpt::Hash(round(a)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		return hash;
	}
}

export using LinearColor = jpt::LinearColor;
template<> constexpr bool jpt::IsTrivial<LinearColor> = true;
