#pragma once

#include <numeric>
#include <stdint.h>

// Constants
static constexpr int32_t kInvalidInt = (std::numeric_limits<int32_t>::max)();
static constexpr float_t kInvalidFloat = (std::numeric_limits<float_t>::max)();

// TypeDefs
using uint8 = uint8_t;
using int32 = int32_t;