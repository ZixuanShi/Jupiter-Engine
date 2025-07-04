// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module jpt.HardwareManager;

import jpt.CPU;

export namespace jpt
{
    class HardwareManager
    {
        JPT_DECLARE_SINGLETON(HardwareManager);

    private:
        CPU m_cpu;

    public:
        bool PreInit();

    public:
        const CPU& GetCPU() const { return m_cpu; }
    };

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