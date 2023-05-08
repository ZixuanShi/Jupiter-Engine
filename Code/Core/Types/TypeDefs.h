#pragma once

#include <numeric>
#include <stdint.h>

// If the value is not supported, you need to specialized template it in the corresponding file
// For example, template<> inline const std::string kDefaultValue<std::string> = "";
template <typename Type>
inline const Type kInvalidValue = (std::numeric_limits<Type>::max)();

template <typename Type>
inline const Type kDefaultValue = static_cast<Type>(0);

// TypeDefs
using uint8  = uint8_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using int32  = int32_t;

// Timing
using TimingPrecision = double;
using Clock        = std::chrono::high_resolution_clock;
using TimePoint    = std::chrono::time_point<Clock>;
using TimeDuration = std::chrono::duration<TimingPrecision>;