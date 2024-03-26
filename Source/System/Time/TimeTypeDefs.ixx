// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"
#include "Debugging/Assert.h"

#include <time.h>
#include <chrono>

export module jpt.Time.TypeDefs;

export namespace jpt
{
	using RawTimeType = time_t;
	using TimePrecision = double;

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