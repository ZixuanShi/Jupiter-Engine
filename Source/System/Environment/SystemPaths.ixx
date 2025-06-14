// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

#if IS_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

export module jpt.SystemPaths;

import jpt.FilePath;
import jpt.FilePathHelpers;

using namespace jpt::File;

export namespace jpt::System
{
    class Paths
    {
    private:
        Path m_engineDir;
        Path m_clientDir;
        Path m_outputDir;
        Path m_savedDir;
        Path m_executablePath;

        bool m_isInitialized = false;

    public:
        JPT_DECLARE_SINGLETON(Paths);
        void PreInit();

        const Path& GetEngineDir();
        const Path& GetClientDir();
        const Path& GetOutputDir();
        const Path& GetSavedDir();
        const Path& GetExecutablePath();

        bool IsInitialized() const { return m_isInitialized; }

    private:
        const wchar_t* GetClientDirW() const;
    };

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

        const size_t lastSlash = m_executablePath.FindLastOf(GetSeparator<Path::TString>());
        m_outputDir = m_executablePath.SubPath(0, lastSlash + 1);

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