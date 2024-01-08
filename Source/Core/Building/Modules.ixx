// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file includes the core minimal utilities non-macro code to run the application. Like typedefs, essential math, jpt.STL, etc. 
// This replaces the deprecated Precompiled Headers in the previous version of Jupiter
// @see https://learn.microsoft.com/en-us/cpp/build/compare-inclusion-methods?view=msvc-170
// quote: "Modules. This is the fastest and most robust way to import functionality." as C++ 23

export module jpt.CoreModules;

// Building
export import jpt.Concepts;
export import jpt.TypeDefs;
export import jpt.Utilities;
export import jpt.Constants;

// Math
export import jpt.Math;
export import jpt.RandomNumberGenerator;

// Memory Managing
export import jpt.Allocator;
export import jpt.UniquePtr;

// Strings
export import jpt.StringUtils;
export import jpt.String;

// Timing
export import jpt.TimingUtils;