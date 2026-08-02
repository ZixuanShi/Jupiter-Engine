// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Logger;

import jpt.TypeDefs;
import std;

namespace jpt::Debug
{
    template<typename... Args>
    struct LogContext
    {
        std::format_string<Args...> format;
        std::source_location location;

        template<typename TString>
        consteval LogContext(const TString& str, std::source_location inLocation = std::source_location::current())
                : format(str)
                , location(inLocation)
            {
            }
    };

    export template<typename... Args>
    void Log(LogContext<std::type_identity_t<Args>...> context, Args&&... args)
    {
        std::string_view fileName = context.location.file_name();
        if (const usize pos = fileName.find("Source/"); pos != std::string_view::npos)
        {
            fileName = fileName.substr(pos);
        }

        const std::string contextStr = std::format("{}({})", fileName, context.location.line());
        const std::string message = std::format(context.format, std::forward<Args>(args)...);

        std::println("{} [Debug]: {}", contextStr, message);
    }
}
