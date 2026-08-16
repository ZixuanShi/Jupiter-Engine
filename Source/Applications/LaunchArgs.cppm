// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.LaunchArgs;

import jpt.TypeDefs;
import std;

namespace jpt::local
{
    struct KeyHash
    {
        using is_transparent = void;

        [[nodiscard]] usize operator()(std::string_view key) const noexcept;
    };
}

export namespace jpt
{
    class LaunchArgs
    {
    public:
        // Will upgrade to Json-like in future, using std::string for now.
        using Data = std::string;

    private:
        std::unordered_map<std::string, Data, local::KeyHash, std::equal_to<>> m_args;

    public:
        void PreInit(int argc, char* argv[]);

    public:
        [[nodiscard]] bool Has(std::string_view key) const noexcept;
    };
}