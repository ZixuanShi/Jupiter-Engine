// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"

export module jpt.Framework_Enums;

import jpt.String;
import jpt.ToString;

export namespace jpt
{
    JPT_ENUM_UINT8(FrameworkAPI, 
        Unknown,
        GLFW,
        Win32)
}

export JPT_ENUM_TO_STRING(jpt::FrameworkAPI);