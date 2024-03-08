// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file is a sheet of constants used by Jupiter Engine

module;

#include <limits>

export module jpt.Constants;

import jpt.Limits;

export namespace jpt
{
	/** Any data sizeof(Type) larger than this will be considered large, and may trigger optimizations */
	constexpr size_t kSmallDataSize = 16;

	/**< If the value is not supported, you need to specialize template it in the corresponding file */
	template<typename T> 
	constexpr T kInvalidValue = LimitsOf<T>::kMax;

	constexpr size_t npos = kInvalidValue<size_t>;
}