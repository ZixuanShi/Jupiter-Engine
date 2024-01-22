// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Pair;

import jpt.Utilities;

export namespace jpt 
{
	template<typename FirstT, typename SecondT>
	struct Pair
	{
	public:
		FirstT first;
		SecondT second;

	public:
		constexpr Pair() = default;
		constexpr Pair(const FirstT& inFirst, const SecondT& inSecond);
		constexpr Pair(const FirstT& inFirst, SecondT&& inSecond);
		constexpr Pair(FirstT&& inFirst, const SecondT& inSecond);
		constexpr Pair(FirstT&& inFirst, SecondT&& inSecond);
	};

	template<typename FirstT, typename SecondT>
	bool operator==(const Pair<FirstT, SecondT>& a, const Pair<FirstT, SecondT>& b)
	{
		return a.first == b.first && a.second == b.second;
	}

	template<typename FirstT, typename SecondT>
	constexpr Pair<FirstT, SecondT>::Pair(const FirstT& inFirst, const SecondT& inSecond)
		: first(inFirst)
		, second(inSecond)
	{
	}

	template<typename FirstT, typename SecondT>
	constexpr Pair<FirstT, SecondT>::Pair(const FirstT& inFirst, SecondT&& inSecond)
		: first(inFirst)
		, second(Move(inSecond))
	{
	}

	template<typename FirstT, typename SecondT>
	constexpr Pair<FirstT, SecondT>::Pair(FirstT&& inFirst, const SecondT& inSecond)
		: first(Move(inFirst))
		, second(inSecond)
	{
	}

	template<typename FirstT, typename SecondT>
	constexpr Pair<FirstT, SecondT>::Pair(FirstT&& inFirst, SecondT&& inSecond)
		: first(Move(inFirst))
		, second(Move(inSecond))
	{
	}
}