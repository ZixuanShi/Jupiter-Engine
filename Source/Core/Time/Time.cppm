// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Time;

import std;

export namespace jpt
{
    /** Local time as YYYYMMDD-HHMMSS -- sortable, and safe in a file name. */
    [[nodiscard]] std::string Timestamp();
}
