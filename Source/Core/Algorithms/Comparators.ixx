// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Comparators;

import jpt.Concepts;

export namespace jpt
{
	template<typename T = void>
	struct Comparator_Less
	{
		template<NonTrivial U = T>
		constexpr bool operator()(const U& lhs, const U& rhs)
		{
			return lhs < rhs;
		}

		template<Trivial U = T>
		constexpr bool operator()(U lhs, U rhs)
		{
			return lhs < rhs;
		}
	};

	template<typename T = void>
	struct Comparator_Greater
	{
		template<NonTrivial U = T>
		constexpr bool operator()(const U& lhs, const U& rhs)
		{
			return lhs > rhs;
		}

		template<Trivial U = T>
		constexpr bool operator()(U lhs, U rhs)
		{
			return lhs > rhs;
		}
	};
}