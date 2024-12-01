// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"
#include "Core/Validation/Assert.h"

#include <corecrt.h>

export module jpt.Time.TypeDefs;

export namespace jpt
{
	using RawTimeType   = time_t;
	using TimePrecision = double;	// For delta time
	using TimeUnit		= int64;	// Milliseconds. For timing units

	consteval TimeUnit operator"" _ms(unsigned long long ms)
	{
		return ms;
	}
	consteval TimeUnit operator"" _s(unsigned long long s)
	{
		return s * 1000;
	}

	JPT_ENUM_UINT8(Month,
		January = 1,	// Start at 1 to match the tm_mon field in the C time library
		February,
		March,
		April,
		May,
		June,
		July,
		August,
		September,
		October,
		November,
		December
	);
}