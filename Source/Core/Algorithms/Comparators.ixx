// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Comparators;

import jpt.Concepts;
import jpt.String.Helpers;

export namespace jpt
{
#pragma region Less
    template<typename T>
    struct Comparator_Less
    {
        constexpr bool operator()(const T& lhs, const T& rhs) const
        {
            return lhs < rhs;
        }
    };
    template<>
    struct Comparator_Less<void>
    {
        template<typename T1, typename T2>
        constexpr bool operator()(const T1& lhs, const T2& rhs) const
        {
            return lhs < rhs;
        }
    };
#pragma endregion

#pragma region Greater
    template<typename T>
    struct Comparator_Greater
    {
        constexpr bool operator()(const T& lhs, const T& rhs) const
        {
            return lhs > rhs;
        }
    };
    template<>
    struct Comparator_Greater<void>
    {
        template<typename T1, typename T2>
        constexpr bool operator()(const T1& lhs, const T2& rhs) const
        {
            return lhs > rhs;
        }
    };
#pragma endregion

#pragma region Equal
    template<typename T>
    struct Comparator_Equal
    {
        constexpr bool operator()(const T& lhs, const T& rhs) const
        {
            return lhs == rhs;
        }
    };
    template<>
    struct Comparator_Equal<void>
    {
        template<typename T1, typename T2>
        constexpr bool operator()(const T1& lhs, const T2& rhs) const
        {
            return lhs == rhs;
        }
    };
    template<>
    struct Comparator_Equal<const char*>
    {
        constexpr bool operator()(const char* lhs, const char* rhs) const
        {
            return AreStringsSame(lhs, rhs);
        }
    };
    template<>
    struct Comparator_Equal<const wchar_t*>
    {
        constexpr bool operator()(const wchar_t* lhs, const wchar_t* rhs) const
        {
            return AreStringsSame(lhs, rhs);
        }
    };
#pragma endregion
}