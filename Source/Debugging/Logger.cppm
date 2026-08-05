// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Logger;

import jpt.DebugContext;
import std;

namespace jpt::Debug
{
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

    export template<typename... Args> void Log  (Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Log,     std::forward<Args>(args)...); }
    export template<typename... Args> void Error(Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Error,   std::forward<Args>(args)...); }
    export template<typename... Args> void Warn (Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Warn, std::forward<Args>(args)...); }
    export template<typename... Args> void Info (Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Info,    std::forward<Args>(args)...); }
}
