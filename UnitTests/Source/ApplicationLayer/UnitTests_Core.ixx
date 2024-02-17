// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Core;

/** Unit Test Modules */

// Minimal
import UnitTests_Macros;

// Algorithms
import UnitTests_Searching;

// Containers
import UnitTests_HashMap;
import UnitTests_LinkedList;
import UnitTests_DynamicArray;
import UnitTests_StaticArray;

// Functional
import UnitTests_Slot;
import UnitTests_Function;

// Utilities
import UnitTests_Utilities;
import UnitTests_TypeTraits;

// Math
import UnitTests_Math;
import UnitTests_RandomNumberGenerator;

// Memory Managing
import UnitTests_Allocator;
import UnitTests_StrongPtr;
import UnitTests_UniquePtr;
import UnitTests_WeakPtr;

// Strings
import UnitTests_StringUtils;
import UnitTests_String;
import UnitTests_StringView;

// Types
import UnitTests_Enum_Global;
import UnitTests_Any;
import UnitTests_Byte;
import UnitTests_Variant;
import UnitTests_Optional;
import UnitTests_Enum;
import UnitTests_Tuple;
import UnitTests_Pair;

export bool RunUnitTests_Core()
{
	/** Unit Test Functions */
	
	// Algorithms
	JPT_ENSURE(RunUnitTests_Searching(), "Algorithms Unit Tests Failed");

	// Containers
	JPT_ENSURE(RunUnitTests_HashMap(), "HashMap Unit Tests Failed");
	JPT_ENSURE(RunUnitTests_LinkedList(), "LinkedList Unit Tests Failed");
	JPT_ENSURE(RunUnitTests_DynamicArray(), "DynamicArray Unit Tests Failed");
	JPT_ENSURE(RunUnitTests_StaticArray(), "StaticArray Unit Tests Failed");

	// Functional
	JPT_ENSURE(RunUnitTests_Slot(), "Slot Unit Tests Failed");
	JPT_ENSURE(RunUnitTests_Function(), "Function Unit Tests Failed");

	// Minimal
	JPT_ENSURE(RunUnitTests_Utilities(), "Utilities Unit Tests Failed");
	JPT_ENSURE(RunUnitTests_TypeTraits(), "TypeTraits Unit Tests Failed");
	JPT_ENSURE(RunUnitTests_Macros(), "Macros Unit Tests Failed");

	// Math
	JPT_ENSURE(RunUnitTests_Math(), "Math Unit Tests Failed");
	JPT_ENSURE(RunUnitTests_RandomNumberGenerator(), "Random Number Generator Tests Failed");

	// Memory Managing
	JPT_ENSURE(RunUnitTests_Allocator(), "Allocator Tests Failed");
	JPT_ENSURE(RunUnitTests_StrongPtr(), "StrongPtr Tests Failed");
	JPT_ENSURE(RunUnitTests_UniquePtr(), "UniquePtr Tests Failed");
	JPT_ENSURE(RunUnitTests_WeakPtr(), "WeakPtr Tests Failed");

	// Strings
	JPT_ENSURE(RunUnitTests_String(), "String Tests Failed");
	JPT_ENSURE(RunUnitTests_StringUtils(), "StringUtils Tests Failed");
	JPT_ENSURE(RunUnitTests_StringView(), "StringView Tests Failed");

	// Types
	JPT_ENSURE(RunUnitTests_Any(), "Any Unit Tests Failed");
	JPT_ENSURE(RunUnitTests_Byte(), "Byte Unit Tests Failed");
	JPT_ENSURE(RunUnitTests_Variant(), "Variant Unit Tests Failed");
	JPT_ENSURE(RunUnitTests_Optional(), "Optional Tests Failed");
	JPT_ENSURE(RunUnitTests_Enum(), "Enum Tests Failed");
	JPT_ENSURE(RunUnitTests_Tuple(), "Tuple Tests Failed");
	JPT_ENSURE(RunUnitTests_Pair(), "Pair Tests Failed");

	return true;
}