// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file includes the core minimal utilities non-macro code to run the application. Like typedefs, essential math, jpt.STL, etc. 
// This replaces the Precompiled Header (PCH) since C++ 20
// @see https://learn.microsoft.com/en-us/cpp/build/compare-inclusion-methods?view=msvc-170
// quote: "Modules. This is the fastest and most robust way to import functionality." as C++ 23

export module jpt.CoreModules;

// Algorithms
export import jpt.LinearFind;
export import jpt.Sort;

// Containers
export import jpt.Deque;
export import jpt.StaticArray;
export import jpt.DynamicArray;
export import jpt.LinkedList;
export import jpt.Stack;
export import jpt.Queue;
export import jpt.HashMap;
export import jpt.HashSet;
export import jpt.SortedMap;
export import jpt.SortedSet;
export import jpt.Heap;

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
export import jpt.Matrix33;
export import jpt.Matrix44;
export import jpt.Quaternion;
export import jpt.Rand;
export import jpt.Line2;
export import jpt.Line3;
export import jpt.LineSegment2;
export import jpt.LineSegment3;
export import jpt.Ray2;
export import jpt.Ray3;
export import jpt.Vector2;
export import jpt.Vector3;
export import jpt.Vector4;
export import jpt.Triangle2;
export import jpt.Triangle3;

// Memory Managing
export import jpt.Allocator;
export import jpt.SharedPtr;
export import jpt.UniquePtr;
export import jpt.WeakPtr;
export import jpt.MemoryPool;

// Strings
export import jpt.String;
export import jpt.String.Helpers;
export import jpt.StringView;
export import jpt.ToString;

// Types
export import jpt.Any;
export import jpt.Byte;
export import jpt.Optional;
export import jpt.Pair;
export import jpt.Tuple;
export import jpt.Variant;
export import jpt.TypeRegistry;