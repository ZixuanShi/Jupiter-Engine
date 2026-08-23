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

    void Output(Level level, const std::source_location& location, const std::string& message)
    {
        const char* levelStr = nullptr;
        switch (level)
        {
            case Level::Log:     levelStr = "LOG";     break;
            case Level::Info:    levelStr = "INFO";    break;
            case Level::Warn:    levelStr = "WARN";    break;
            case Level::Error:   levelStr = "ERROR";   break;
        }

        const std::string line = std::format("{}({}) [{}]: {}", TrimFileName(location.file_name()),
                                             location.line(), levelStr, message);

#if IS_PLATFORM_ANDROID
        // One fixed priority: the level lives in the text, the same line every platform prints.
        __android_log_write(ANDROID_LOG_INFO, "Jupiter", line.c_str());
#else
        std::println("{}", line);
#endif
    }
}
