// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module jpt.Input.Create;

import jpt.RawInput;
import jpt.RawInput_GLFW;
import jpt.RawInput_Win32;

import jpt.Framework.Enums;

export namespace jpt::Input
{
    RawInput* RawInput_Create(Framework_API frameworkAPI)
    {
        switch (frameworkAPI.Value())
        {
        case Framework_API::GLFW:
            return new RawInput_GLFW();

        case Framework_API::Win32:
            return new RawInput_Win32();

        default:
            JPT_WARN("Unsupported framework API: " + ToString(frameworkAPI));
            return nullptr;
        }
    }
}