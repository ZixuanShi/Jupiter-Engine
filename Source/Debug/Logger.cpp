// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

// `stdout` is a macro, and `import std;` carries no macros.
#include <cstdio>

#if IS_PLATFORM_ANDROID
    #include <android/log.h>
#endif

module jpt.Logger;

import std;

namespace jpt::Debug
{
    void PreInit()
    {
#if IS_PLATFORM_WINDOWS
        // Measured, not read from the spec: the UCRT maps _IOLBF onto _IOFBF, so a
        // newline-terminated line was still in the buffer when the process was killed. Unbuffered
        // is the only mode it honours, and the cost is one write per line on the platform that
        // draws nothing yet.
        std::setvbuf(stdout, nullptr, _IONBF, 0);
#else
        std::setvbuf(stdout, nullptr, _IOLBF, 0);
#endif
    }

    void Output(Level level, const std::string& contextStr, const std::string& message)
    {
#if IS_PLATFORM_ANDROID
        // The priority carries the level, so the line does not repeat it.
        android_LogPriority priority = ANDROID_LOG_INFO;
        switch (level)
        {
            case Level::Log:     priority = ANDROID_LOG_DEBUG;   break;
            case Level::Info:    priority = ANDROID_LOG_INFO;    break;
            case Level::Warn:    priority = ANDROID_LOG_WARN;    break;
            case Level::Error:   priority = ANDROID_LOG_ERROR;   break;
        }
        __android_log_write(priority, "Jupiter", std::format("{}: {}", contextStr, message).c_str());
#else
        const char* levelStr = nullptr;
        switch (level)
        {
            case Level::Log:     levelStr = "LOG";     break;
            case Level::Info:    levelStr = "INFO";    break;
            case Level::Warn:    levelStr = "WARN";    break;
            case Level::Error:   levelStr = "ERROR";   break;
        }
        std::println("{} [{}]: {}", contextStr, levelStr, message);
#endif
    }
}
