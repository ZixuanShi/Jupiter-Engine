// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <type_traits>
#include <iostream>

export module jpt.Concepts;

import jpt.Constants;
import jpt.TypeTraits;

export namespace jpt
{
#pragma region Type Defs

	template<typename T>
	concept Numeric = std::is_arithmetic_v<T>;

	template<typename T>
	concept Integral = std::is_integral_v<T>;

	template<typename T>
	concept Floating = std::is_floating_point_v<T>;

	template<typename T>
	concept StringLiteral = AreSameType<T, char> || AreSameType<T, wchar_t>;

	template<typename T>
	concept Primitive = std::is_fundamental_v<T>;

	// If a simple struct is deduced as non-trivial, you can override jpt::IsTrivial<T> to return true in your struct. Refer to Vector2
	template<typename T>
	concept Trivial = jpt::IsTrivial<T> &&
					  sizeof(T) <= kSmallDataSize;

	template<typename T>
	concept NonTrivial = !jpt::IsTrivial<T> ||
		                 sizeof(T) > kSmallDataSize;

#pragma endregion

#pragma region Constructing
	
	template<typename T>
	concept MoveAssignable = std::is_move_assignable_v<T>;

	template<typename T>
	concept MoveConstructible = IsConstructible<T, T>;

	template<typename T>
	concept Movable = MoveConstructible<T> && MoveAssignable<T>;

#pragma endregion

#pragma region Comparing
	template<typename T>
	concept Comparable = requires(T left, T right)
	{
		left < right;
		left > right;
	};

	/** Avoid copy-constructing when comparing non-trivially copiable objects */
	template<typename T>
	concept ComparableTrivial = Comparable<T> && Trivial<T>;
	template<typename T>
	concept ComparableNonTrivial = Comparable<T> && NonTrivial<T>;

#pragma endregion

#pragma region Container
	template<typename T>
	concept Iterable = requires(T container)
	{
		container.begin();
		container.end();
	};

	template<typename T>
	concept Sized = requires(T container)
	{
		container.Size();
	};

	template<typename TContainer>
	concept Containing = Iterable<TContainer> && Sized<TContainer>;

	template<typename TContainer>
	concept ContainingTrivial = Containing<TContainer> && Trivial<typename TContainer::TData>;

	template<typename TContainer>
	concept ContainingNonTrivial = Containing<TContainer> && NonTrivial<typename TContainer::TData>;
#pragma endregion
}