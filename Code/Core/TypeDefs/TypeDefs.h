#pragma once

#include <numeric>
#include <stdint.h>

// Constants
template <typename Type>
inline const Type kInvalidValue = (std::numeric_limits<Type>::max)();

template <typename Type>
inline const Type kDefaultValue = static_cast<Type>(0);

template<>
inline const std::string kDefaultValue<std::string> = "";

// TypeDefs
using uint8 = uint8_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using int32 = int32_t;