// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Slot;

import jpt.Slot;
import jpt.TypeDefs;
import jpt.Variant;
import jpt.DynamicArray;
import jpt.ToString;
import jpt.Utilities;
import jpt.TypeTraits;

int32 Add(int32 a, int32 b)
{
	return a + b;
}

struct Test
{
	int32 Subtract(int32 a, int32 b)
	{
		return a - b;
	}
};

bool UnitTest_Slot()
{
	jpt::Slot<int32(int32, int32)> slot;

	// Adding
	// global function
	size_t addIndex = slot.Add(&Add);

	// member function
	Test test;
	size_t subIndex = slot.Add(&test, &Test::Subtract);

	// Lambda
	slot.Add([](int32 a, int32 b)
		{
			return a * b;
		});

	auto division = [](int32 a, int32 b)
		{
		return a / b;
	};
	slot.Add(division);

	// Erasing
	slot.Erase(addIndex);
	addIndex = slot.Add(&Add);
	JPT_ENSURE(addIndex == 0);

	slot.Erase(subIndex);
	subIndex = slot.Add(&test, &Test::Subtract);
	JPT_ENSURE(subIndex == 1);

	// Calling
	slot(1, 2);
	jpt::DynamicArray<int32> results = slot.ReturnAll(6, 2);
	JPT_ENSURE(results[0] == 8);
	JPT_ENSURE(results[1] == 4);
	JPT_ENSURE(results[2] == 12);
	JPT_ENSURE(results[3] == 3);

	slot.Erase(addIndex);
	slot.Erase(subIndex);

	size_t i = slot.Add([](int32 a, int32 b) { return a * 2 + b; });
	JPT_ENSURE(i == 0);
	slot.Erase(i);

	results = slot.ReturnAll(6, 2);
	JPT_ENSURE(results[0] == 0);
	JPT_ENSURE(results[1] == 0);
	JPT_ENSURE(results[2] == 12);
	JPT_ENSURE(results[3] == 3);

	return true;
}

void VoidFunction(int32& i)
{
	i *= 2;
}

bool UnitTest_Slot_Void()
{
	jpt::Slot<void(int32&)> voidSlots;

	voidSlots.Add([](int32& i)
		{
			i *= 2;
		});

	voidSlots.Add(&VoidFunction);

	int32 i = 2;
	voidSlots(i);
	JPT_ENSURE(i == 8);

	return true;
}

bool UnitTest_Slot_Function()
{
	jpt::Slot<void(int32&)> slot;

	jpt::Function<void(int32&)> adder        = [](int32& i) { i += 2; };
	jpt::Function<void(int32&)> subbstracter = [](int32& i) { i -= 2; };
	jpt::Function<void(int32&)> multiplier   = [](int32& i) { i *= 2; };
	jpt::Function<void(int32&)> divider      = [](int32& i) { i /= 2; };

	slot.Add(adder);
	slot.Add(multiplier);
	slot.Add(subbstracter);
	slot.Add(divider);

	int32 i = 2;
	slot(i);

	JPT_ENSURE(i == 3);

	return true;
}

export bool RunUnitTests_Slot()
{
	JPT_ENSURE(UnitTest_Slot());
	JPT_ENSURE(UnitTest_Slot_Void());
	JPT_ENSURE(UnitTest_Slot_Function());

    return true;
}
