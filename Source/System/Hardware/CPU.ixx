// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_WIN64
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#include <thread>

export module jpt.CPU;

import jpt.TypeDefs;
import jpt.String;

export namespace jpt
{
    class CPU
    {
    private:
        uint32 m_logicalProcessorsCount = 0;
        uint32 m_coresCount = 0;

    public:
        bool PreInit();

    public:
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

        free(buffer);

		return true;
    }
}