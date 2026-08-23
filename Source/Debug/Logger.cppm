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

    /** Composes the line and hands it to the platform's sink -- stdout everywhere but Android,
        where stdout goes nowhere and lines land in logcat instead. The one place a Level means
        anything. Body in Logger.cpp. */
    void Output(Level level, const std::string& contextStr, const std::string& message);

    template<typename... Args>
    void Impl(Context<std::type_identity_t<Args>...> context, Level level, Args&&... args)
    {
        const std::string_view fileName = TrimFileName(context.location.file_name());
        Output(level, std::format("{}({})", fileName, context.location.line()),
               std::format(context.format, std::forward<Args>(args)...));
    }

    export template<typename... Args> void Log  (Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Log,   std::forward<Args>(args)...); }
    export template<typename... Args> void Error(Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Error, std::forward<Args>(args)...); }
    export template<typename... Args> void Warn (Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Warn,  std::forward<Args>(args)...); }
    export template<typename... Args> void Info (Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Info,  std::forward<Args>(args)...); }
}
