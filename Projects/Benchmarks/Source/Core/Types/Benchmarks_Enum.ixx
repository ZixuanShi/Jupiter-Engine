// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module Benchmarks_Enum;

import jpt.TypeDefs;
import jpt.Utilities;

JPT_ENUM_UINT8(EFruit,
	Apple,
	Orange,
	Banana,
	Strawberry,
	Blueberry,
	Cherry,
	Grape,
	Pear,
	Peach,
	Watermelon,
	Pineapple,
	Mango,
	Kiwi,
	)

void Benchmarks_Enum_Init()
{
}

export void RunBenchmarks_Enum()
{
	Benchmarks_Enum_Init();

}