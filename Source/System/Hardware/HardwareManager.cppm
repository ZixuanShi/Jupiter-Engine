// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Utilities.h"

export module jpt.HardwareManager;

import jpt.TypeDefs;
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

    uint32 GetLogicalProcessorsCount()
    {
        return HardwareManager::GetInstance().GetCPU().GetLogicalProcessorsCount();
    }

    uint32 GetCoresCount()
    {
        return HardwareManager::GetInstance().GetCPU().GetCoresCount();
    }
}