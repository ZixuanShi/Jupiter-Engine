// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file is a sheet of constants used by Jupiter Engine

module;

#include <limits>

export module jpt.Constants;

import jpt.Limits;
import jpt.TypeDefs;

export namespace jpt
{
	/** Any data sizeof(Type) larger than this will be considered large, and may trigger optimizations */
	constexpr size_t kSmallDataSize = 16;

	/**< If the value is not supported, you need to specialize template it in the corresponding file */
	template<typename T> 
	constexpr T kInvalidValue = Limits<T>::kMax;

	/** Short version of invalid value regarding indexing */
	constexpr size_t npos = kInvalidValue<size_t>;

	/** Grow multiplier for dynamic containers */
	constexpr size_t kGrowMultiplier = 2;

	template<typename T = float>
	constexpr T kPi = static_cast<T>(3.14159265358979323846f);
}