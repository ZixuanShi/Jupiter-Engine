// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Tuple;

import jpt.Utilities;
import jpt.TypeTraits;

/** Recursive Inheritance implementation  */
export namespace jpt
{
    template<typename... TArgs>
    struct Tuple {};

    template<typename TData, typename... TRest>
    struct Tuple<TData, TRest...> : public Tuple<TRest...>
    {
        TData m_value;

    public:
        constexpr Tuple() = default;
        constexpr Tuple(const TData& value, TRest&&... rest)
            : Tuple<TRest...>(Forward<TRest>(rest)...)
            , m_value(value)
        {
        }
    };
}

/** Helpers for retrieving Tuple data */
namespace jpt_private
{
    using jpt::Tuple;

    template<size_t kIndex, typename TData, typename... TRest>
    struct GetImpl
    {
        using TNext = GetImpl<kIndex - 1, TRest...>;

        static constexpr auto Value(const Tuple<TData, TRest...>& tuple) -> decltype(TNext::Value(tuple))
        {
            return TNext::Value(tuple);
        }
    };

    template<typename TData, typename... TRest>
    struct GetImpl<0, TData, TRest...>
    {
        static constexpr TData& Value(const Tuple<TData, TRest...>& tuple)
        {
            return const_cast<TData&>(tuple.m_value);
        }
    };
}

/** Tuple global APIs */
namespace jpt
{
    /** Get Value at Index */
    using jpt_private::GetImpl;
    export template<size_t index, typename TData, typename... TRest>
    constexpr auto Get(const Tuple<TData, TRest...>& tuple) -> decltype(GetImpl<index, TData, TRest...>::Value(tuple))
    {
        return GetImpl<index, TData, TRest...>::Value(tuple);
    }

    /** Getting how many data Ts can a tuple holds */
    export template<typename... TArgs>
    consteval size_t GetSize(const Tuple<TArgs...>& /*tuple*/)
    {
        return sizeof...(TArgs);
    }

    /** Forwarding a group of data as tuple */
    export template<typename... TArgs>
    constexpr Tuple<TArgs...> Tie(TArgs&&... args)
    {
        return Tuple<TArgs...>(Forward<TArgs>(args)...);
    }
}