// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
    /** Lights the shader's uniform block has room for, and the size of the engine's arrays. */
    enum : unsigned int { kMaxPointLights = 3 };

    /** Frames the CPU may run ahead by.  */
    enum : unsigned int { kFramesInFlight = 3 };
}
