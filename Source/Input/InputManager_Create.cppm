// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module jpt.InputManager_Create;

import jpt.RawInput;
import jpt.RawInput_GLFW;
import jpt.RawInput_Win32;

import jpt.Framework_Enums;

export namespace jpt::Input
{
    RawInput* RawInput_Create(FrameworkAPI frameworkAPI)
    {
        switch (frameworkAPI.Value())
        {
        case FrameworkAPI::GLFW:
            return Allocator<RawInput_GLFW>::New();

        case FrameworkAPI::Win32:
            return Allocator<RawInput_Win32>::New();

        default:
            JPT_WARN("Unsupported framework API: " + ToString(frameworkAPI));
            return nullptr;
        }
    }
}