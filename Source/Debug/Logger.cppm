// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Logger;

import jpt.DebugContext;
import std;

namespace jpt::Debug
{
    /** Makes a log line leave the process as it is written rather than when a 4 KB block fills.
        Two things depend on it: `devicectl --console` hands an iOS app a pipe, which is
        block-buffered by default and showed nothing on device; and Debug::Assert traps immediately
        after logging, so an unflushed message is one that never arrives. Which stdio mode delivers
        that is per-platform -- see Logger.cpp. */
    export void PreInit();

    enum class Level
    {
        Log,
        Info,
        Warn,
        Error,
    };

    template<typename... Args>
    void Impl(Context<std::type_identity_t<Args>...> context, Level level, Args&&... args)
    {
        const std::string_view fileName = TrimFileName(context.location.file_name());
        const std::string contextStr = std::format("{}({})", fileName, context.location.line());
        const std::string message = std::format(context.format, std::forward<Args>(args)...);

        const char* levelStr = nullptr;
        switch (level)
        {
            case Level::Log:     levelStr = "LOG";     break;
            case Level::Info:    levelStr = "INFO";    break;
            case Level::Warn:    levelStr = "WARN";    break;
            case Level::Error:   levelStr = "ERROR";   break;
        }

        std::println("{} [{}]: {}", contextStr, levelStr, message);
    }

    export template<typename... Args> void Log  (Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Log,   std::forward<Args>(args)...); }
    export template<typename... Args> void Error(Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Error, std::forward<Args>(args)...); }
    export template<typename... Args> void Warn (Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Warn,  std::forward<Args>(args)...); }
    export template<typename... Args> void Info (Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Info,  std::forward<Args>(args)...); }
}
