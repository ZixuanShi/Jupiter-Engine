// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Utilities.h"
#include "Debugging/Logger.h"

module jpt.HardwareManager;

namespace jpt
{
    bool HardwareManager::PreInit()
    {
        JPT_ENSURE(m_cpu.PreInit());

        JPT_INFO("CPU: " + m_cpu.GetName());

        return true;
    }

    uint32 GetLogicalProcessorsCount()
    {
        return HardwareManager::GetInstance().GetCPU().GetLogicalProcessorsCount();
    }

    uint32 GetCoresCount()
    {
        return HardwareManager::GetInstance().GetCPU().GetCoresCount();
    }
}