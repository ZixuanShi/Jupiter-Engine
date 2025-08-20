// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Utilities.h"

export module jpt.SystemPaths;

import jpt.FilePath;

export namespace jpt::System
{
    class Paths
    {
    private:
        File::Path m_engineDir;
        File::Path m_clientDir;
        File::Path m_outputDir;
        File::Path m_savedDir;
        File::Path m_executablePath;

        bool m_isInitialized = false;

    public:
        JPT_DECLARE_SINGLETON(Paths);
        void PreInit();

        const File::Path& GetEngineDir();
        const File::Path& GetClientDir();
        const File::Path& GetOutputDir();
        const File::Path& GetSavedDir();
        const File::Path& GetExecutablePath();

        bool IsInitialized() const { return m_isInitialized; }

    private:
        const wchar_t* GetClientDirW() const;
    };
}