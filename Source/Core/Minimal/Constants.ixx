// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file is a sheet of constants used by Jupiter Engine

module;

#include <limits>

export module jpt.Constants;

import jpt.TypeDefs;

export namespace jpt
{
	template<typename T>
	struct Constants
	{
		static constexpr T kMax = std::numeric_limits<T>::max();
		static constexpr T kMin = std::numeric_limits<T>::min();
		static constexpr T kEpsilon = std::numeric_limits<T>::epsilon();
	};

	/** Any data sizeof(Type) larger than this will be considered large, and may trigger optimizations */
	constexpr size_t kSmallDataSize = 16;

	/**< If the value is not supported, you need to specialize template it in the corresponding file */
	template<typename T> 
	constexpr T kInvalidValue = Constants<T>::kMax;

	/** Short version of invalid value regarding indexing */
	constexpr size_t npos = kInvalidValue<size_t>;

	/** Grow multiplier for dynamic containers */
	constexpr size_t kGrowMultiplier = 2;

	template<typename T = float>
	constexpr T kPi = static_cast<T>(3.14159265358979323846f);
}

export constexpr float PI = jpt::kPi<float>;
export constexpr double PI_D = jpt::kPi<double>;
