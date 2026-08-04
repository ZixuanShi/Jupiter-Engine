// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <Foundation/Foundation.hpp>

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
