// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Enum.h"

#include "Debugging/Logger.h"

using namespace jpt;

void EnumBuilder(jpt::DynamicArray<jpt::String>& names, uint8 count, const char* pSource)
{
	String source(pSource);
	source.Replace(" ", "");

	names.Resize(count);
	names = source.Split(',');
}
