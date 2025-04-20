// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <stdint.h>

export module jpt.TypeDefs;

export using uint8    = uint8_t;
export using uint16   = uint16_t;
export using uint32   = uint32_t;
export using uint64   = uint64_t;

export using int8     = int8_t;
export using int16    = int16_t;
export using int32    = int32_t;
export using int64    = int64_t;

export using float32  = float;
export using float64  = double;

export using Id            = size_t;    // Unique ID for objects
export using Index         = size_t;    // Index for containers
export using Precision     = float32;
export using TimePrecision = float32;

export using uintptr = uintptr_t;

export enum class EIntBase : uint8
{
    Decimal = 10,
    Hex     = 16,
    Binary  = 2,
    Octal   = 8
};