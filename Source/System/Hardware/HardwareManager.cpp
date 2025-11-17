// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

module jpt.HardwareManager;

namespace jpt
{
    bool HardwareManager::PreInit()
    {
        JPT_ENSURE(m_cpu.PreInit());

        JPT_INFO("CPU: " + m_cpu.GetName());

        return true;
    }
}