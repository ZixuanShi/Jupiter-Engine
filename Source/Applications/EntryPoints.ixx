// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_WIN64
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

export module jpt.EntryPoints;

import jpt.Application;
import jpt.CommandLine;

#if IS_DEBUG
    import jpt.MemoryLeakDetector;
#endif

namespace jpt
{
    // Called by platform-specific entry points
    int MainImpl()
    {
#if IS_DEBUG
        MemoryLeakDetector::Init();
#endif

        Application* pApp = GetApplication();
        if (pApp->PreInit() && pApp->Init())
        {
            pApp->Run();
        }

        pApp->Shutdown();

        return 0;
    }
}

// Platform-specific entry points
// Should handle command line arguments and call MainImpl_Final
#if IS_PLATFORM_WIN64

import jpt.Platform.Win64;

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR launchArgs, int nCmdShow)
{
    using namespace jpt;

    CommandLine::GetInstance().Parse(launchArgs);

    Platform_Win64* pWin64 = new Platform_Win64(hInstance, nCmdShow);
    GetApplication()->SetPlatform(pWin64);

    return MainImpl();
}
#else
int main(int argc, char* argv[])
{
    using namespace jpt;

    CommandLine::GetInstance().Parse(argc, argv);
    return jpt::MainImpl();
}
#endif
