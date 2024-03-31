// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file includes the core minimal utilities non-macro code to run the application. Like typedefs, essential math, jpt.STL, etc. 
// This replaces the deprecated Precompiled Headers in the previous version of Jupiter
// @see https://learn.microsoft.com/en-us/cpp/build/compare-inclusion-methods?view=msvc-170
// quote: "Modules. This is the fastest and most robust way to import functionality." as C++ 23

export module jpt.CoreModules;

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
export import jpt.Math;
export import jpt.Limits;
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
export import jpt.StringView;
export import jpt.ToString;

// Types
export import jpt.Optional;
export import jpt.Tuple;
export import jpt.Pair;