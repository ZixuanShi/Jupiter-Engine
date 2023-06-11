// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	/* Color represented by 4 floats for each channel, RGBA order. */
	class JPT_API LinearColor
	{
	public:
		float r = kDefaultValue<float>;
		float g = kDefaultValue<float>;
		float b = kDefaultValue<float>;
		float a = kDefaultValue<float>;

	public:
		LinearColor() = default;
		LinearColor(float value);
		LinearColor(float _r, float _g, float _b, float _a);
	};

	/* Color represented by 4 8-bit usigned int for each channel, RGBA order. */
	class JPT_API UintColor
	{
	public:
		uint8 r = kDefaultValue<uint8>;
		uint8 g = kDefaultValue<uint8>;
		uint8 b = kDefaultValue<uint8>;
		uint8 a = kDefaultValue<uint8>;

	public:
		UintColor() = default;
		UintColor(uint8 value);
		UintColor(uint8 _r, uint8 _g, uint8 _b, uint8 _a);
	};
}