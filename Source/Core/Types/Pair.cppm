// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <compare>

export module jpt.Pair;

import jpt.Utilities;
import jpt.Serializer;

export namespace jpt 
{
    template<typename TFirst, typename TSecond>
    struct Pair
    {
    public:
        TFirst first;
        TSecond second;

    public:
        constexpr Pair() = default;
        constexpr Pair(const TFirst& inFirst, const TSecond& inSecond);
        constexpr Pair(const TFirst& inFirst, TSecond&& inSecond);
        constexpr Pair(TFirst&& inFirst, const TSecond& inSecond);
        constexpr Pair(TFirst&& inFirst, TSecond&& inSecond);

        void Serialize(Serializer& serializer) const;
        void Deserialize(Serializer& serializer);
    };

    // ------------------------------------------------------------------------------------------------
    // Non-Member functions
    // ------------------------------------------------------------------------------------------------
    template<typename TFirst, typename TSecond>
    constexpr bool operator==(const Pair<TFirst, TSecond>& a, const Pair<TFirst, TSecond>& b)
    {
        return a.first == b.first && a.second == b.second;
    }


    // ------------------------------------------------------------------------------------------------
    // Member functions
    // ------------------------------------------------------------------------------------------------
    template<typename TFirst, typename TSecond>
    constexpr Pair<TFirst, TSecond>::Pair(const TFirst& inFirst, const TSecond& inSecond)
        : first(inFirst)
        , second(inSecond)
    {
    }

    template<typename TFirst, typename TSecond>
    constexpr Pair<TFirst, TSecond>::Pair(const TFirst& inFirst, TSecond&& inSecond)
        : first(inFirst)
        , second(Move(inSecond))
    {
    }

    template<typename TFirst, typename TSecond>
    constexpr Pair<TFirst, TSecond>::Pair(TFirst&& inFirst, const TSecond& inSecond)
        : first(Move(inFirst))
        , second(inSecond)
    {
    }

    template<typename TFirst, typename TSecond>
    constexpr Pair<TFirst, TSecond>::Pair(TFirst&& inFirst, TSecond&& inSecond)
        : first(Move(inFirst))
        , second(Move(inSecond))
    {
    }

    template<typename TFirst, typename TSecond>
    void Pair<TFirst, TSecond>::Serialize(Serializer& serializer) const
    {
        serializer.Write(first);
        serializer.Write(second);
    }

    template<typename TFirst, typename TSecond>
    void Pair<TFirst, TSecond>::Deserialize(Serializer& serializer)
    {
        serializer.Read(first);
        serializer.Read(second);
    }
}