// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_WIN64
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#include <thread>
#include <intrin.h>

export module jpt.CPU;

import jpt.TypeDefs;
import jpt.String;
import jpt.StaticArray;

export namespace jpt
{
    class CPU
    {
    private:
        String m_name;
        uint32 m_logicalProcessorsCount = 0;
        uint32 m_coresCount = 0;

    public:
        bool PreInit();

    public:
		const String& GetName() const { return m_name; }
        uint32 GetLogicalProcessorsCount() const { return m_logicalProcessorsCount; }
        uint32 GetCoresCount() const { return m_coresCount; };
    };

    bool CPU::PreInit()
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        // Get number of logical processors
        m_logicalProcessorsCount = static_cast<uint32>(sysInfo.dwNumberOfProcessors);

        // To get physical cores, use GetLogicalProcessorInformation
        DWORD length = 0;
        GetLogicalProcessorInformation(nullptr, &length);
        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer =
            (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(length);
        GetLogicalProcessorInformation(buffer, &length);

        // Count physical cores
        for (DWORD i = 0; i < length / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i)
        {
            if (buffer[i].Relationship == RelationProcessorCore)
            {
                ++m_coresCount;
            }
        }

        // Get CPU Name
       StaticArray<int32, 4> cpuInfo;
       m_name.Resize(64);

        // Get extended ids
        __cpuid(cpuInfo.Buffer(), 0x80000000);
        uint32_t nExIds = cpuInfo[0];

        // Get the CPU brand string using extended CPUID
        for (uint32_t i = 0x80000002; i <= 0x80000004; ++i)
        {
            if (i <= nExIds)
            {
                __cpuid(cpuInfo.Buffer(), i);
                memcpy(m_name.Buffer() + (i - 0x80000002) * 16, cpuInfo.Buffer(), sizeof(cpuInfo));
            }
        }

        // Free resources
        free(buffer);

		return true;
    }
}