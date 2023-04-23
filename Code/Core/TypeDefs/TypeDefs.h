#pragma once

#include <numeric>
#include <stdint.h>

// Constants
template <typename Type>
static constexpr Type kInvalid = (std::numeric_limits<Type>::max)();

// TypeDefs
using uint8 = uint8_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using int32 = int32_t;