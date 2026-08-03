// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Environment;

export namespace jpt
{
    consteval const char* GetPlatformName()
    {
        #if IS_PLATFORM_WINDOWS
            return "Windows";
        #elif IS_PLATFORM_MACOS
            return "macOS";
        #elif IS_PLATFORM_IOS
            return "iOS";
        #elif IS_PLATFORM_ANDROID
            return "Android";
        #elif IS_PLATFORM_LINUX
            return "Linux";
        #else
            static_assert(false, "Unknown platform");
            return "Unknown";
        #endif
    }

    consteval const char* GetConfigName()
    {
        #if IS_CONFIG_DEBUG
            return "Debug";
        #elif IS_CONFIG_DEV
            return "Dev";
        #elif IS_CONFIG_RELEASE
            return "Release";
        #else
            static_assert(false, "Unknown configuration");
            return "Unknown";
        #endif
    }
}
