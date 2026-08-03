// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Assert;

import jpt.DebugContext;
import jpt.Logger;
import std;

export namespace jpt::Debug
{
    /** Reads like Log(), because it shares Log()'s Context: the reported location is the call
        site, not this file.

        A function rather than a macro, which costs one thing worth knowing: arguments are
        always evaluated, even in Release where the body is discarded. Keep the condition cheap.

        @return the condition, so callers can branch on it without testing twice. */
    template<typename... Args>
    bool Assert(bool condition, Context<std::type_identity_t<Args>...> context, Args&&... args)
    {
        if constexpr (!IS_CONFIG_RELEASE)
        {
            if (!condition) [[unlikely]]
            {
                Error(context, std::forward<Args>(args)...);

                // Clang builtin rather than __debugbreak(), which is Windows only.
                __builtin_debugtrap();
            }
        }

        return condition;
    }
}
