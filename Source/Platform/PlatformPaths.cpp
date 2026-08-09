// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
    #include <Foundation/Foundation.hpp>
#elif IS_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
#endif

module jpt.PlatformPaths;

import std;

namespace jpt
{
    const std::filesystem::path& GetRoot()
    {
#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
        // Contents/Resources on macOS, the bundle root on iOS. Asking the bundle rather than
        // deriving it from the executable path keeps that difference Apple's problem.
        static const std::filesystem::path root(NS::Bundle::mainBundle()->resourcePath()->utf8String());
        return root;
#elif IS_PLATFORM_WINDOWS
        // The executable's own directory, which is where CMake stages Assets/ -- see
        // JUPITER_ASSET_DIR. Not argv[0]: a caller is free to put anything there.
        static const std::filesystem::path root = []
        {
            std::wstring buffer(MAX_PATH, L'\0');
            for (;;)
            {
                const DWORD written = GetModuleFileNameW(nullptr, buffer.data(),
                                                         static_cast<DWORD>(buffer.size()));
                if (written == 0)
                {
                    return std::filesystem::current_path();
                }
                if (written < buffer.size())
                {
                    buffer.resize(written);
                    break;
                }
                buffer.resize(buffer.size() * 2);   // Truncated: the path is longer than MAX_PATH.
            }

            return std::filesystem::path(buffer).parent_path();
        }();

        return root;
#else
        #error "No Jupiter root for this platform"
#endif
    }

    const std::filesystem::path& GetSavedDir()
    {
        static const std::filesystem::path saved = []
        {
#ifdef JUPITER_SAVED_DIR
            // A desktop dev build: the repo's _Saved, so clean.py can wipe it and the files sit
            // somewhere you can actually look at them.
            std::filesystem::path directory(JUPITER_SAVED_DIR);
    #elif IS_PLATFORM_MACOS || IS_PLATFORM_IOS
            // $HOME, because metal-cpp's Foundation subset does not bind NSFileManager. Same
            // place either way: the home directory, or the app container on iOS.
            const char* pHome = std::getenv("HOME");
            std::filesystem::path directory(pHome ? pHome : ".");
            directory /= "Library/Application Support/JupiterEngine";
    #elif IS_PLATFORM_WINDOWS
            // %LOCALAPPDATA%, not %APPDATA%: this is a cache of one machine's preferences and
            // logs, and nothing here is worth roaming.
            const char* pLocalAppData = std::getenv("LOCALAPPDATA");
            std::filesystem::path directory(pLocalAppData ? pLocalAppData : ".");
            directory /= "JupiterEngine";
    #else
            #error "No saved directory for this platform"

#endif // JUPITER_SAVED_DIR

            std::error_code error;
            std::filesystem::create_directories(directory, error);
            return directory;
        }();

        return saved;
    }
}
