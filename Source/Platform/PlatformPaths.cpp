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
        //
        // Resolved once: it hits the filesystem, and every path in the engine is built from it.
        static const std::filesystem::path root(NS::Bundle::mainBundle()->resourcePath()->utf8String());
        return root;
#else
        #error "No Jupiter root for this platform"
#endif
    }
}
