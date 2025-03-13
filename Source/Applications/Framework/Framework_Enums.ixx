// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"

export module jpt.Framework.Enums;

import jpt.String;
import jpt.ToString;

export namespace jpt
{
    JPT_ENUM_UINT8(Framework_API, 
        Unknown,
        GLFW,
        Win32)
}

export JPT_ENUM_TO_STRING(jpt::Framework_API);