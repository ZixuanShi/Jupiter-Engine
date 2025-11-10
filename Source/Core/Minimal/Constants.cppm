// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file is a sheet of constants used by Jupiter Engine

module;

#include <limits>

export module jpt.Constants;

import jpt.TypeDefs;

export namespace jpt
{
    template<typename T>
    constexpr T kMax = std::numeric_limits<T>::max();

    template<typename T>
    constexpr T kMin = std::numeric_limits<T>::min();

    /**< If the value is not supported, you need to specialize template it in the corresponding file */
    template<typename T> 
    constexpr T kInvalidValue = kMax<T>;

    /** Any data sizeof(Type) larger than this will be considered large, and may trigger optimizations */
    constexpr size_t kSmallDataSize = 16;

    template<typename T = Precision>
    constexpr T kEpsilon = static_cast<T>(1e-6);

    template<typename T = Precision>
    constexpr T kPi = static_cast<T>(3.14159265358979323846f);

    template<typename T = Precision>
    constexpr T kTwoPi = static_cast<T>(6.28318530717958647692f);

    template<typename T = Precision>
    constexpr T kHalfPi = static_cast<T>(1.57079632679489661923f);
}

export constexpr Precision Pi = jpt::kPi<Precision>;
export constexpr Precision TwoPi = jpt::kTwoPi<Precision>;
export constexpr Precision HalfPi = jpt::kHalfPi<Precision>;

export constexpr Id    kInvalidId    = jpt::kInvalidValue<Id>;
export constexpr Index kInvalidIndex = jpt::kInvalidValue<Index>;

