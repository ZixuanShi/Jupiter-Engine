// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

module jpt.LaunchArgs;

import jpt.Logger;

namespace jpt
{
    /** Flags only this pass: "-no_window" is stored as "no_window", so Has() is asked for the name
        without its dashes. A key-value pass follows once Data is a real Json-like type. */
    void LaunchArgs::PreInit(int argc, char* argv[])
    {
        if (argc > 0 && argv == nullptr)
        {
            return;
        }

        m_args.reserve(static_cast<usize>(argc));

        // From 1: argv[0] is the executable path, not an argument.
        for (int i = 1; i < argc; ++i)
        {
            const std::string_view token = argv[i];
            const usize nameStart = token.find_first_not_of('-');

            // 0 is a bare token, npos is "-"/"--"/empty. Neither names a flag.
            if (nameStart == 0 || 
                nameStart == std::string_view::npos)
            {
                Debug::Warn("Ignoring launch arg \"{}\": a flag is one or more '-' then a name.", token);
                continue;
            }

            m_args.try_emplace(std::string(token.substr(nameStart)));
        }
    }

    bool LaunchArgs::Has(std::string_view key) const noexcept
    {
        return m_args.contains(key);
    }

    namespace local
    {
        usize KeyHash::operator()(std::string_view key) const noexcept
        {
            return std::hash<std::string_view>{}(key);
        }
    }
}