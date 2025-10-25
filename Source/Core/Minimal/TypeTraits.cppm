// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <type_traits>

export module jpt.TypeTraits;

import jpt.Constants;

export namespace jpt
{
#pragma region Type Identification
    /** @return true if the Type is within any of the passed ...Types
        @example:
                int foo = 42;
                double bar = 42.0;
                jpt::IsAnyOf<decltype(foo), float, char, int>; // true
                jpt::IsAnyOf<decltype(bar), float, char, int>; // false    */
    template<typename T, typename ...TOthers> 
    constexpr bool IsAnyOf  = (std::is_same_v<T, TOthers> || ...);

    /** @return true if decltype(foo) is "string" */
    template<typename T> constexpr bool IsCharArray                = false;
    template<size_t N>   constexpr bool IsCharArray<char[N]>       = true;
    template<size_t N>   constexpr bool IsCharArray<const char[N]> = true;

#pragma endregion

#pragma region Type Properties
    template<typename T>    constexpr bool IsTrivial = std::is_trivial_v<T>;
    template<typename T>    constexpr bool IsSmall = sizeof(T) <= kSmallDataSize;

#pragma endregion
}