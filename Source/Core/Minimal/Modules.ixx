// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file includes the core minimal utilities non-macro code to run the application. Like typedefs, essential math, jpt.STL, etc. 
// This replaces the Precompiled Header (PCH) since C++ 20
// @see https://learn.microsoft.com/en-us/cpp/build/compare-inclusion-methods?view=msvc-170
// quote: "Modules. This is the fastest and most robust way to import functionality." as C++ 23

export module jpt.CoreModules;

// Algorithms
export import jpt.Searching;
export import jpt.Sorting;

// Containers
export import jpt.StaticArray;
export import jpt.DynamicArray;
export import jpt.HashMap;

// Functional
export import jpt.Function;
export import jpt.Slot;

// Minimals
export import jpt.Concepts;
export import jpt.Constants;
export import jpt.TypeDefs;
export import jpt.TypeTraits;
export import jpt.Utilities;

// Math
export import jpt.Limits;
export import jpt.Math;
export import jpt.RandomNumberGenerator;
export import jpt.Vector2;
export import jpt.Vector3;

// Memory Managing
export import jpt.Allocator;
export import jpt.StrongPtr;
export import jpt.UniquePtr;
export import jpt.WeakPtr;

// Strings
export import jpt.String;
export import jpt.StringUtils;
export import jpt.StringView;
export import jpt.ToString;

// Types
export import jpt.Any;
export import jpt.Byte;
export import jpt.Optional;
export import jpt.Pair;
export import jpt.Tuple;
export import jpt.Variant;