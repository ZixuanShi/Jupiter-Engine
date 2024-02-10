// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Core;

/** Unit Test Modules */

// Algorithms
import UnitTests_Searching;

// Containers
import UnitTests_HashMap;
import UnitTests_LinkedList;
import UnitTests_DynamicArray;
import UnitTests_StaticArray;

// Utilities
import UnitTests_Utilities;
import UnitTests_TypeTraits;

// Debugging
import UnitTests_Logger;

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
import UnitTests_Byte;
import UnitTests_Variant;
import UnitTests_Function;
import UnitTests_Optional;
import UnitTests_Enum;
import UnitTests_Tuple;
import UnitTests_Pair;

export bool RunUnitTests_Core()
{
	/** Unit Test Functions */
	
	// Algorithms
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_Searching(), "Algorithms Unit Tests Failed");

	// Containers
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_HashMap(), "HashMap Unit Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_LinkedList(), "LinkedList Unit Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_DynamicArray(), "DynamicArray Unit Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_StaticArray(), "StaticArray Unit Tests Failed");

	// Minimals
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_Utilities(), "Utilities Unit Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_TypeTraits(), "TypeTraits Unit Tests Failed");

	// Debugging
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_Logger(), "Logger Unit Tests Failed");

	// Math
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_Math(), "Math Unit Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_RandomNumberGenerator(), "Random Number Generator Tests Failed");

	// Memory Managing
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_Allocator(), "Allocator Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_StrongPtr(), "StrongPtr Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_UniquePtr(), "UniquePtr Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_WeakPtr(), "WeakPtr Tests Failed");

	// Strings
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_String(), "String Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_StringUtils(), "StringUtils Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_StringView(), "StringView Tests Failed");

	// Types
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_Byte(), "Byte Unit Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_Variant(), "Variant Unit Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_Function(), "Function Unit Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_Optional(), "Optional Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_Enum(), "Enum Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_Tuple(), "Tuple Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_Pair(), "Pair Tests Failed");

	return true;
}