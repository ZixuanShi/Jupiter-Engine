// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.DebugContext;

import std;

export namespace jpt::Debug
{
    /** Pairs a format string with where it was written. The constructor is consteval so the
        format is checked at compile time, and the defaulted source_location resolves at the
        call site rather than here. Shared by Log() and Assert(). */
    template<typename... Args>
    struct Context
    {
        std::format_string<Args...> format;
        std::source_location location;

        template<typename TString>
        consteval Context(const TString& str, std::source_location inLocation = std::source_location::current());
    };

    template<typename... Args>
    template<typename TString>
    consteval Context<Args...>::Context(const TString& str, std::source_location inLocation)
        : format(str)
        , location(inLocation)
    {
    }

    /** Trims the absolute path down to the repository-relative part. */
    constexpr std::string_view TrimFileName(std::string_view fileName)
    {
        if (const std::size_t pos = fileName.find("Source/"); pos != std::string_view::npos)
        {
            return fileName.substr(pos);
        }
        return fileName;
    }
}
