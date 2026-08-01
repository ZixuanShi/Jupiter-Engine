// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.TypeDefs;

import std;

export using uint8  = std::uint8_t;
export using uint16 = std::uint16_t;
export using uint32 = std::uint32_t;
export using uint64 = std::uint64_t;

export using int8  = std::int8_t;
export using int16 = std::int16_t;
export using int32 = std::int32_t;
export using int64 = std::int64_t;

export using float32 = float;
export using float64 = double;

export using usize = std::size_t;

export namespace jpt
{
    enum class Status : uint8
    {
        Pending,        // Not started
        Running,        // In progress
        Succeeded,      // Completed successfully
        Failed,         // Completed with errors
        Warning,        // Completed with warnings
        Canceled,       // Canceled by user
    };
}