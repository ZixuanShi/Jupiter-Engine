// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
    /** Frames the CPU may run ahead by. */
    enum : unsigned int { kFramesInFlight = 3 };

    /** MSAA samples per pixel. */
    enum : unsigned int { kSampleCount = 4 };
}
