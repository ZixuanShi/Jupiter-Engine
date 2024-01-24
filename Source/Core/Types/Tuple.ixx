// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Tuple;

import jpt.Utilities;
import jpt.TypeTraits;

/** Recursive Inheritance implementation  */
export namespace jpt
{
	template<typename... ArgsT>
	struct Tuple {};

	template<typename DataT, typename... RestT>
		struct Tuple<DataT, RestT...> : public Tuple<RestT...>
	{
	public:
		DataT m_value;

	public:
		constexpr Tuple() = default;
		constexpr Tuple(const DataT& value, RestT&&... rest)
			: Tuple<RestT...>(Forward<RestT>(rest)...)
			, m_value(value)
		{
		}
	};
}

/** Helpers for retrieving Tuple data */
namespace jpt_private
{
	using jpt::Tuple;

	template<size_t index, typename DataT, typename... Rest>
	struct GetImpl
	{
		using NextT = GetImpl<index - 1, Rest...>;

		static auto Value(const Tuple<DataT, Rest...>& tuple) -> decltype(NextT::Value(tuple))
		{
			return NextT::Value(tuple);
		}
	};

	template<typename DataT, typename... Rest>
	struct GetImpl<0, DataT, Rest...>
	{
		static DataT& Value(const Tuple<DataT, Rest...>& tuple)
		{
			return const_cast<DataT&>(tuple.m_value);
		}
	};
}

/** Tuple global APIs */
namespace jpt
{
	using jpt_private::GetImpl;

	/** Get Value at Index */
	export template<size_t index, typename DataT, typename... Rest>
	constexpr auto Get(const Tuple<DataT, Rest...>& tuple) -> decltype(GetImpl<index, DataT, Rest...>::Value(tuple))
	{
		return GetImpl<index, DataT, Rest...>::Value(tuple);
	}

	/** Forwarding a group of data as tuple */
	export template<typename... ArgsT>
	constexpr Tuple<ArgsT...> Tie(ArgsT&&... args)
	{
		return Tuple<ArgsT...>(Forward<ArgsT>(args)...);
	}

	/** Getting how many data Ts can a tuple holds */
	export template <typename DataT>
	struct TupleSize 
	{
	};

	export template<typename... ArgsT>
	struct TupleSize<Tuple<ArgsT...>>
	{ 
		static constexpr size_t kValue = sizeof...(ArgsT);
	};

	export template<typename... ArgsT>
	struct TupleSize<const Tuple<ArgsT...>>
	{
		static constexpr size_t kValue = sizeof...(ArgsT);
	};	
}