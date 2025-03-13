// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Environment;

export namespace jpt
{
    consteval const char* GetPlatformName()
    {
#if IS_PLATFORM_WIN64
        return "Win64";
#elif IS_PLATFORM_LINUX
        return "Linux";
#elif IS_PLATFORM_MAC
        return "Mac";
#elif IS_PLATFORM_ANDROID
        return "Android";
#elif IS_PLATFORM_IOS
        return "iOS";
#elif IS_PLATFORM_PLAYSTATION
        return "PlayStation";
#elif IS_PLATFORM_XBOX
        return "Xbox";
#elif IS_PLATFORM_SWITCH
        return "Switch";
#else
#error "Unknown platform"
#endif
    }
}