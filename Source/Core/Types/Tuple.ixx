// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Tuple;

import jpt.Utilities;
import jpt.TypeTraits;

export namespace jpt
{
	template<typename DataT, typename... RestT>
	struct Tuple : public Tuple<RestT...>
	{
	public:
		DataT m_value;

	public:
		constexpr Tuple() = default;
		constexpr Tuple(const DataT& value, RestT&&... rest);
	};

	template<typename DataT>
	struct Tuple<DataT>
	{
	public:
		DataT m_value;

	public:
		constexpr Tuple() = default;
		constexpr Tuple(const DataT& value);
	};

	template<typename DataT, typename ...RestT>
	constexpr Tuple<DataT, RestT...>::Tuple(const DataT& value, RestT&& ...rest)
		: Tuple<RestT...>(Forward<RestT>(rest)...)
		, m_value(value)
	{
	}

	template<typename DataT>
	constexpr Tuple<DataT>::Tuple(const DataT& value)
		: m_value(value)
	{
	}
}

/** Helpers for retrieving Tuple data */
namespace jpt_private
{
	using jpt::Tuple;

	template<int index, typename DataT, typename... Rest>
	struct GetValueAtImpl
	{
		static auto Value(const Tuple<DataT, Rest...>& t) -> decltype(GetValueAtImpl<index - 1, Rest...>::Value(t))&
		{
			return GetValueAtImpl<index - 1, Rest...>::Value(t);
		}
	};

	template<typename DataT, typename... Rest>
	struct GetValueAtImpl<0, DataT, Rest...>
	{
		static DataT& Value(const Tuple<DataT, Rest...>& t)
		{
			return *const_cast<DataT*>(&t.m_value);
		}
	};
}

/** Tuple global APIs */
export namespace jpt
{
	// Get Value at index
	using jpt_private::GetValueAtImpl;

	template<int index, typename DataT, typename... Rest>
	auto GetValueAt(const Tuple<DataT, Rest...>& t) -> decltype(GetValueAtImpl<index, DataT, Rest...>::Value(t))&
	{
		return GetValueAtImpl<index, DataT, Rest...>::Value(t);
	}

	/** Forwarding a group of data as tuple */
	template<typename... Ts>
	constexpr Tuple<Ts...> Tie(Ts&&... args)
	{
		return Tuple<Ts...>(Forward<Ts>(args)...);
	}

	/** Getting how many data Ts can a tuple holds */
	template <typename T> 
	struct TupleSize {};

	template<typename... Ts>
	struct TupleSize<Tuple<Ts...>> 
	{ 
		static constexpr size_t kValue = sizeof...(Ts);
	};

	template<typename... Ts>
	struct TupleSize<const Tuple<Ts...>>
	{
		static constexpr size_t kValue = sizeof...(Ts);
	};
}