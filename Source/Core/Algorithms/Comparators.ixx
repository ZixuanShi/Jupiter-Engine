// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Comparators;

import jpt.Concepts;
import jpt.StringUtils;

export namespace jpt
{
	template<NonTrivial T>
	struct Comparator_Less
	{
		constexpr bool operator()(const T& lhs, const T& rhs) const
		{
			return lhs < rhs;
		}
	};
	template<Trivial T>
	struct Comparator_Less<T>
	{
		constexpr bool operator()(T lhs, T rhs) const
		{
			return lhs < rhs;
		}
	};
	template<>
	struct Comparator_Less<void>
	{
		template<NonTrivial T>
		constexpr bool operator()(const T& lhs, const T& rhs) const
		{
			return lhs < rhs;
		}
		template<Trivial T>
		constexpr bool operator()(T lhs, T rhs) const
		{
			return lhs < rhs;
		}
	};

	template<NonTrivial T>
	struct Comparator_Greater
	{
		constexpr bool operator()(const T& lhs, const T& rhs) const
		{
			return lhs > rhs;
		}
	};
	template<Trivial T>
	struct Comparator_Greater<T>
	{
		constexpr bool operator()(T lhs, T rhs) const
		{
			return lhs > rhs;
		}
	};
	template<>
	struct Comparator_Greater<void>
	{
		template<NonTrivial T>
		constexpr bool operator()(const T& lhs, const T& rhs) const
		{
			return lhs > rhs;
		}
		template<Trivial T>
		constexpr bool operator()(T lhs, T rhs) const
		{
			return lhs > rhs;
		}
	};

	template<typename T = void>
	struct Comparator_Equal
	{
		template<NonTrivial U = T>
		constexpr bool operator()(const U& lhs, const U& rhs) const
		{
			return lhs == rhs;
		}

		template<Trivial U = T>
		constexpr bool operator()(U lhs, U rhs) const
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
}