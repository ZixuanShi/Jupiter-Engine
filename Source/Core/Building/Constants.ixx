// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file is a sheet of constants used by Jupiter Engine

export module jpt.Constants;

export namespace jpt
{
	/** Any class over 16 bytes will be considered large, and may trigger optimizations */
	constexpr size_t kLargeClassSize = 16;
}