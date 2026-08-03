// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Logger;

import jpt.TypeDefs;
import std;

namespace jpt::Debug
{
    enum class Level
    {
        Log,        // Verbose logging, for development and debugging. Excluded for production.
        Info,       // Important information, included in production.
        Warning,
        Error,
    };

    template<typename... Args>
    struct Context
    {
        std::format_string<Args...> format;
        std::source_location location;

        template<typename TString>
        consteval Context(const TString& str, std::source_location inLocation = std::source_location::current())
                : format(str)
                , location(inLocation)
            {
            }
    };

    template<typename... Args>
    void Impl(Context<std::type_identity_t<Args>...> context, Level level, Args&&... args)
    {
        std::string_view fileName = context.location.file_name();
        if (const usize pos = fileName.find("Source/"); pos != std::string_view::npos)
        {
            fileName = fileName.substr(pos);
        }

        const std::string contextStr = std::format("{}({})", fileName, context.location.line());
        const std::string message = std::format(context.format, std::forward<Args>(args)...);

        const char* levelStr = nullptr;
        switch (level)
        {
            case Level::Log:     levelStr = "LOG";     break;
            case Level::Info:    levelStr = "INFO";    break;
            case Level::Warning: levelStr = "WARNING"; break;
            case Level::Error:   levelStr = "ERROR";   break;
        }

        std::println("{} [{}]: {}", contextStr, levelStr, message);
    }

    export template<typename... Args> void Log    (Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Log,     std::forward<Args>(args)...); }
    export template<typename... Args> void Error  (Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Error,   std::forward<Args>(args)...); }
    export template<typename... Args> void Warning(Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Warning, std::forward<Args>(args)...); }
    export template<typename... Args> void Info   (Context<std::type_identity_t<Args>...> context, Args&&... args) { Impl(context, Level::Info,    std::forward<Args>(args)...); }
}
