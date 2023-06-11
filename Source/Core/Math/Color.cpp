// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "Color.h"

namespace jpt
{
	LinearColor::LinearColor(float value)
		: r(value)
		, g(value)
		, b(value)
		, a(value)
	{
	}

	LinearColor::LinearColor(float _r, float _g, float _b, float _a)
		: r(_r)
		, g(_g)
		, b(_b)
		, a(_a)
	{
	}

	UintColor::UintColor(uint8 value)
		: r(value)
		, g(value)
		, b(value)
		, a(value)
	{
	}

	UintColor::UintColor(uint8 _r, uint8 _g, uint8 _b, uint8 _a)
		: r(_r)
		, g(_g)
		, b(_b)
		, a(_a)
	{
	}
}