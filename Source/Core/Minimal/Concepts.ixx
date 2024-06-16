// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <type_traits>
#include <iostream>
#include <fstream>

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
	concept Trivial = jpt::IsTrivial<T> && jpt::IsSmall<T>;

	template<typename T>
	concept NonTrivial = !jpt::IsTrivial<T> || !jpt::IsSmall<T>;

#pragma endregion

#pragma region Constructing
	
	template<typename T>
	concept MoveAssignable = std::is_move_assignable_v<T>;

	template<typename T>
	concept MoveConstructible = std::is_constructible_v<T, T>;

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
	concept Indexable = requires(T& obj, size_t i)
	{
		obj[i];
	};

	template<typename T>
	concept Bufferred = requires(T obj)
	{
		obj.Buffer();
		obj.ConstBuffer();
	};

	template<typename T>
	concept Countable = requires(T obj)
	{
		obj.Count();
	};

	template<typename T>
	concept Containable = Iterable<T> && Countable<T> && requires(T t)
	{
		typename T::TData;
	};

	template<typename TContainer>
	concept IndexableTrivial = Indexable<TContainer> && Trivial<typename TContainer::TData>;

	template<typename TContainer>
	concept IndexableNonTrivial = Indexable<TContainer> && NonTrivial<typename TContainer::TData>;
#pragma endregion Container

	template<typename T>
	concept Serializable = requires(T obj, std::ifstream& is, std::ofstream& os)
	{
		obj.Serialize(os);
		obj.Deserialize(is);
	};
}