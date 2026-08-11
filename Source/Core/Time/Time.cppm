// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Time;

import jpt.TypeDefs;
import std;

export namespace jpt
{
    /** Local time as YYYYMMDD-HHMMSS -- sortable, and safe in a file name. */
    [[nodiscard]] std::string Timestamp();

    /** Platform event timestamps arrive in nanoseconds -- SDL's do -- while everything the engine
        measures time in is seconds. float64 because a nanosecond count is well past float32's
        24-bit mantissa within a second of uptime. */
    [[nodiscard]] float64 NanoSecsToSeconds(uint64 nanoseconds) noexcept;
}
