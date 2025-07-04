// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_CONFIG_DEBUG

    #if IS_PLATFORM_WINDOWS
        #define _CRTDBG_MAP_ALLOC
        #include <stdlib.h>
        #include <crtdbg.h>
    #endif

#endif // IS_CONFIG_DEBUG

export module jpt.MemoryLeakDetector;

#if IS_CONFIG_DEBUG

namespace jpt::MemoryLeakDetector
{
    export void Init()
    {
        #if IS_PLATFORM_WINDOWS
            _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
            _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
        #endif
    }
}

#endif // IS_CONFIG_DEBUG