// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#if IS_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

module jpt.SystemPaths;

import jpt.FilePathHelpers;

namespace jpt::System
{
    using namespace jpt::File;

    void Paths::PreInit()
    {
        if (m_isInitialized)
        {
            return;
        }

        m_engineDir = JPT_ENGINE_DIR_W;
        m_clientDir = GetClientDirW();

#if IS_PLATFORM_WINDOWS
        wchar_t buffer[MAX_PATH];
        GetModuleFileNameW(nullptr, buffer, MAX_PATH);
        m_executablePath = buffer;
#endif

        m_outputDir = m_executablePath.GetParent();

        m_savedDir = m_outputDir + L"_Saved/";

        JPT_ASSERT(!m_engineDir.IsEmpty());
        JPT_ASSERT(!m_clientDir.IsEmpty());
        JPT_ASSERT(!m_outputDir.IsEmpty());
        JPT_ASSERT(!m_savedDir.IsEmpty());
        JPT_ASSERT(!m_executablePath.IsEmpty());

        m_isInitialized = true;
    }

    const Path& Paths::GetEngineDir()
    {
        if (!m_isInitialized)
        {
            PreInit();
        }
        return m_engineDir;
    }

    const Path& Paths::GetClientDir()
    {
        if (!m_isInitialized)
        {
            PreInit();
        }
        return m_clientDir;
    }

    const Path& Paths::GetOutputDir()
    {
        if (!m_isInitialized)
        {
            PreInit();
        }
        return m_outputDir;
    }

    const Path& Paths::GetSavedDir()
    {
        if (!m_isInitialized)
        {
            PreInit();
        }
        return m_savedDir;
    }

    const Path& Paths::GetExecutablePath()
    {
        if (!m_isInitialized)
        {
            PreInit();
        }
        return m_executablePath;
    }
}