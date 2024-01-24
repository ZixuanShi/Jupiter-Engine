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

	template<size_t index, typename DataT, typename... RestT>
	struct GetImpl
	{
		using NextT = GetImpl<index - 1, RestT...>;

		static auto Value(const Tuple<DataT, RestT...>& tuple) -> decltype(NextT::Value(tuple))
		{
			return NextT::Value(tuple);
		}
	};

	template<typename DataT, typename... RestT>
	struct GetImpl<0, DataT, RestT...>
	{
		static DataT& Value(const Tuple<DataT, RestT...>& tuple)
		{
			return const_cast<DataT&>(tuple.m_value);
		}
	};

	template<class... ArgsT>
	struct GetSizeImpl
	{
		static constexpr size_t kValue = sizeof...(ArgsT);
	};
}

/** Tuple global APIs */
namespace jpt
{
	/** Get Value at Index */
	using jpt_private::GetImpl;

	export template<size_t index, typename DataT, typename... RestT>
	constexpr auto Get(const Tuple<DataT, RestT...>& tuple) -> decltype(GetImpl<index, DataT, RestT...>::Value(tuple))
	{
		return GetImpl<index, DataT, RestT...>::Value(tuple);
	}

	/** Getting how many data Ts can a tuple holds */
	using jpt_private::GetSizeImpl;

	export template<typename... ArgsT>
	consteval size_t GetSize(const jpt::Tuple<ArgsT...>& tuple)
	{
		return GetSizeImpl<ArgsT...>::kValue;
	}

	/** Forwarding a group of data as tuple */
	export template<typename... ArgsT>
	constexpr Tuple<ArgsT...> Tie(ArgsT&&... args)
	{
		return Tuple<ArgsT...>(Forward<ArgsT>(args)...);
	}
}