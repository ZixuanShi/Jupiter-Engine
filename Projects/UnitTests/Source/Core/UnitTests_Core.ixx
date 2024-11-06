// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Core;

import jpt.CommandLine;
import jpt.Utilities;

/** Unit Test Modules */

// Algorithms
import UnitTests_BinarySearch;
import UnitTests_Comparators;
import UnitTests_Sorting;
import UnitTests_Searching;

// Containers
import UnitTests_KDTree3;
import UnitTests_Queue;
import UnitTests_Stack;
import UnitTests_SortedMap;
import UnitTests_HashSet;
import UnitTests_HashMap;
import UnitTests_LinkedList;
import UnitTests_DynamicArray;
import UnitTests_StaticArray;
import UnitTests_Heap;
import UnitTests_Graph;

// Functional
import UnitTests_Slot;
import UnitTests_Function;

// Math
import UnitTests_Vector4;
import UnitTests_Color;
import UnitTests_Box3;
import UnitTests_Box2;
import UnitTests_Ray2;
import UnitTests_LineSegment2;
import UnitTests_Line2;
import UnitTests_Triangle3;
import UnitTests_Triangle2;
import UnitTests_Hash;
import UnitTests_Vector3;
import UnitTests_Vector2;
import UnitTests_Math;
import UnitTests_Rand;
import UnitTests_MathPractice;
import UnitTests_Matrix33;
import UnitTests_Matrix44;
import UnitTests_Quaternion;

// Memory Managing
import UnitTests_Allocator;
import UnitTests_SharedPtr;
import UnitTests_UniquePtr;
import UnitTests_WeakPtr;

// Minimal
import UnitTests_Concepts;
import UnitTests_Macros;
import UnitTests_Utilities;
import UnitTests_TypeTraits;

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

// Threading
import UnitTests_Threading;

export bool RunUnitTests_Core()
{
	/** Unit Test Functions */
	
	// Algorithms
	JPT_ENSURE(RunUnitTests_BinarySearch());
	JPT_ENSURE(RunUnitTests_Comparators());
	JPT_ENSURE(RunUnitTests_Sorting());
	JPT_ENSURE(RunUnitTests_Searching());

	// Containers
	JPT_ENSURE(RunUnitTests_KDTree3());
	JPT_ENSURE(RunUnitTests_Queue());
	JPT_ENSURE(RunUnitTests_Stack());
	JPT_ENSURE(RunUnitTests_SortedMap());
	JPT_ENSURE(RunUnitTests_StaticArray());
	JPT_ENSURE(RunUnitTests_DynamicArray());
	JPT_ENSURE(RunUnitTests_LinkedList());
	JPT_ENSURE(RunUnitTests_HashMap());
	JPT_ENSURE(RunUnitTests_HashSet());
	JPT_ENSURE(RunUnitTests_Heap());
	JPT_ENSURE(RunUnitTests_Graph());

	// Functional
	JPT_ENSURE(RunUnitTests_Slot());
	JPT_ENSURE(RunUnitTests_Function());

	// Minimal
	JPT_ENSURE(RunUnitTests_Concepts());
	JPT_ENSURE(RunUnitTests_Utilities());
	JPT_ENSURE(RunUnitTests_TypeTraits());
	JPT_ENSURE(RunUnitTests_Macros());

	// Math
	JPT_ENSURE(RunUnitTests_Vector4());
	JPT_ENSURE(RunUnitTests_Color());
	JPT_ENSURE(RunUnitTests_Box3());
	JPT_ENSURE(RunUnitTests_Box2());
	JPT_ENSURE(RunUnitTests_Ray2());
	JPT_ENSURE(RunUnitTests_LineSegment2());
	JPT_ENSURE(RunUnitTests_Line2());
	JPT_ENSURE(RunUnitTests_Triangle3());
	JPT_ENSURE(RunUnitTests_Triangle2());
	JPT_ENSURE(RunUnitTests_Hash());
	JPT_ENSURE(RunUnitTests_Vector3());
	JPT_ENSURE(RunUnitTests_Vector2());
	JPT_ENSURE(RunUnitTests_Math());
	JPT_ENSURE(RunUnitTests_Rand());
	JPT_ENSURE(RunUnitTests_Matrix33());
	JPT_ENSURE(RunUnitTests_Matrix44());
	JPT_ENSURE(RunUnitTests_Quaternion());

	// Memory Managing
	JPT_ENSURE(RunUnitTests_Allocator());
	JPT_ENSURE(RunUnitTests_SharedPtr());
	JPT_ENSURE(RunUnitTests_UniquePtr());
	JPT_ENSURE(RunUnitTests_WeakPtr());

	// Strings
	JPT_ENSURE(RunUnitTests_String());
	JPT_ENSURE(RunUnitTests_StringUtils());
	JPT_ENSURE(RunUnitTests_StringView());

	// Types
	JPT_ENSURE(RunUnitTests_Any());
	JPT_ENSURE(RunUnitTests_Byte());
	JPT_ENSURE(RunUnitTests_Variant());
	JPT_ENSURE(RunUnitTests_Optional());
	JPT_ENSURE(RunUnitTests_Enum());
	JPT_ENSURE(RunUnitTests_Tuple());
	JPT_ENSURE(RunUnitTests_Pair());

	// Threading
	if (!jpt::CommandLine::GetInstance().Has("no_threading"))
	{
		JPT_ENSURE(RunUnitTests_Threading());
	}

	return true;
}