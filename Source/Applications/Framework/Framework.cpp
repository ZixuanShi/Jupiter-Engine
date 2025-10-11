// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"
#include "Core/Minimal/Utilities.h"

module jpt.Framework;

namespace jpt
{
    bool Framework::PreInit()
    {
        JPT_ENSURE(m_api != FrameworkAPI::Unknown);

        return true;
    }
}