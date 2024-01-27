// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file is a sheet of constants used by Jupiter Engine

module;

#include <limits>

export module jpt.Constants;

export namespace jpt
{
	/** Any data sizeof(Type) larger than this will be considered large, and may trigger optimizations */
	constexpr size_t kLargeDataSize = 16;

	template<typename T> const T kInvalidValue = (std::numeric_limits<T>::max)();	/**< If the value is not supported, you need to specialize template it in the corresponding file */

	constexpr size_t npos = kInvalidValue<size_t>;
}