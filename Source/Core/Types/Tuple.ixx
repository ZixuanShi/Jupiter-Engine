// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Tuple;

import jpt.Utilities;
import jpt.TypeTraits;

export namespace jpt
{
	/** Recurssive inheriting tuple implementation */
	template<typename... Args>
	struct Tuple
	{
		static constexpr bool kIsConstructible = true;
	};

	template<typename DataT, typename... OtherTs>
	struct Tuple<DataT, OtherTs...> : public Tuple<OtherTs...>
	{
	public:
		static constexpr bool kIsConstructible = IsConstructible<DataT> && Tuple<OtherTs...>::kIsConstructible;

		DataT m_value;	/**< The value at this DataT & index will be stored */

	public:
		constexpr Tuple() = default;
		constexpr Tuple(DataT value, OtherTs... otherValues);
	};

	template<typename DataT, typename ...OtherTs>
	constexpr Tuple<DataT, OtherTs...>::Tuple(DataT value, OtherTs ...otherValues)
		: Tuple<OtherTs...>(Forward<OtherTs>(otherValues)...)
		, m_value(Forward<DataT>(value))
	{
	}
}

/** Helpers for getting value */
namespace jpt_private
{
	using jpt::Tuple;

	template<size_t, typename T>
	struct GetTypeAt;

	template<typename CurrentT, typename... OtherTs>
	struct GetTypeAt<0, Tuple<CurrentT, OtherTs...>>
	{
		using Type      = CurrentT;
		using TupleType = Tuple<CurrentT, OtherTs...>;
	};

	template<size_t Index, typename CurrentT, typename... OtherTs>
	struct GetTypeAt<Index, Tuple<CurrentT, OtherTs...>>
	{
		using Type      = GetTypeAt<Index - 1, Tuple<OtherTs...>>::Type;
		using TupleType = GetTypeAt<Index - 1, Tuple<OtherTs...>>::TupleType;
	};
}

/** Global utilities functions around tuple */
export namespace jpt
{
	/** Retrieving data from a tuple */
	template<size_t Index, typename... Ts>
	constexpr jpt_private::GetTypeAt<Index, Tuple<Ts...>>::Type& GetValueAt(Tuple<Ts...>& tuple)
	{
		using TupleType = jpt_private::GetTypeAt<Index, Tuple<Ts...>>::TupleType;

		return reinterpret_cast<TupleType&>(tuple).m_value;
	}

	template<size_t Index, typename... Ts>
	constexpr const jpt_private::GetTypeAt<Index, Tuple<Ts...>>::Type& GetValueAt(const Tuple<Ts...>& tuple)
	{
		using TupleType = jpt_private::GetTypeAt<Index, Tuple<Ts...>>::TupleType;

		return reinterpret_cast<const TupleType&>(tuple).m_value;
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