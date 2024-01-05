// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file is a sheet of constants used by Jupiter Engine

export module jpt.Constants;

export namespace jpt
{
	/** Any data sizeof(Type) larger than this will be considered large, and may trigger optimizations */
	constexpr size_t kLargeDataSize = 16;
}