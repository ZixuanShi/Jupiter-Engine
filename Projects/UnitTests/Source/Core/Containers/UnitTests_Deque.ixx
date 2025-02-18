// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module UnitTests_Deque;

import jpt.Deque;
import jpt.Utilities;
import jpt.TypeDefs;
import jpt.String;

auto locHelper = [](size_t i) -> const char*
	{
		switch (i)
		{
		case 0: return "Zero";
		case 1: return "One";
		case 2: return "Two";
		case 3: return "Three";
		case 4: return "Four";
		case 5: return "Five";
		case 6: return "Six";
		case 7: return "Seven";
		case 8: return "Eight";
		case 9: return "Nine";
		case 10: return "Ten";
		case 11: return "Eleven";
		case 12: return "Twelve";

		default: return "Error";
		}
	};

bool UnitTests_Deque_Trivial()
{
	jpt::Deque<int32, 8> deque;

	// Adding
	// Increasing order
	deque.AddFront(3);
	deque.AddFront(2);
	deque.AddFront(1);
	deque.AddFront(0);
	deque.AddBack(4);
	deque.AddBack(5);
	deque.AddBack(6);
	deque.AddBack(7);

	JPT_ENSURE(deque.IsFull());

	for (int32 i = 0; i < deque.Capacity(); ++i)
	{
		JPT_ENSURE(deque.Front() == i);
		deque.PopFront();
	}

	JPT_ENSURE(deque.IsEmpty());

	// Decreasing order
	deque.AddBack(7);
	deque.AddBack(6);
	deque.AddBack(5);
	deque.AddBack(4);
	deque.AddBack(3);
	deque.AddBack(2);
	deque.AddBack(1);
	deque.AddBack(0);

	JPT_ENSURE(deque.IsFull());

	// Copy
	jpt::Deque<int32, 8> deque2;
	deque2 = deque;

	for (int32 i = 0; i < deque.Capacity(); ++i)
	{
		JPT_ENSURE(deque.Back() == i);
		deque.PopBack();
	}

	JPT_ENSURE(deque.IsEmpty());

	for (int32 i = 0; i < deque2.Capacity(); ++i)
	{
		JPT_ENSURE(deque2.Back() == i);
		deque2.PopBack();
	}

	return true;
}

bool UnitTests_Deque_NonTrivial()
{
	jpt::Deque<jpt::String, 8> deque;

	// Adding
	// Increasing order
	deque.AddFront("Three");
	deque.AddFront("Two");
	deque.AddFront("One");
	deque.AddFront("Zero");
	deque.AddBack("Four");
	deque.AddBack("Five");
	deque.AddBack("Six");
	deque.AddBack("Seven");

	JPT_ENSURE(deque.IsFull());

	for (int32 i = 0; i < deque.Capacity(); ++i)
	{
		JPT_ENSURE(deque.Front() == locHelper(i));
		deque.PopFront();
	}

	JPT_ENSURE(deque.IsEmpty());

	// Decreasing order
	deque.AddBack("Seven");
	deque.AddBack("Six");
	deque.AddBack("Five");
	deque.AddBack("Four");
	deque.AddBack("Three");
	deque.AddBack("Two");
	deque.AddBack("One");
	deque.AddBack("Zero");

	JPT_ENSURE(deque.IsFull());

	jpt::Deque<jpt::String, 8> deque2;
	deque2 = deque;

	for (int32 i = 0; i < deque.Capacity(); ++i)
	{
		JPT_ENSURE(deque.Back() == locHelper(i));
		deque.PopBack();
	}

	JPT_ENSURE(deque.IsEmpty());

	// Move
	deque = jpt::Move(deque2);
	JPT_ENSURE(deque2.IsEmpty());
	JPT_ENSURE(deque.IsFull());

	for (int32 i = 0; i < deque.Capacity(); ++i)
	{
		JPT_ENSURE(deque.Back() == locHelper(i));
		deque.PopBack();
	}

	return true;
}

export bool RunUnitTests_Deque()
{
	JPT_ENSURE(UnitTests_Deque_Trivial());
	JPT_ENSURE(UnitTests_Deque_NonTrivial());

	return true;
}