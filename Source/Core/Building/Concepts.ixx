// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <type_traits>
#include <iostream>

export module jpt.Concepts;

import jpt.Constants;
import jpt.TypeDefs;

export namespace jpt
{
	template<typename Type>
	concept Numeric = std::is_arithmetic_v<Type>;

	template<typename Type>
	concept Integral = std::is_integral_v<Type>;

	template<typename Type>
	concept Floating = std::is_floating_point_v<Type>;

	template<typename Type>
	concept StringLiteral = IsSameType<Type, char>::Value || IsSameType<Type, wchar_t>::Value;

	template<typename Type>
	concept Copyable = std::is_copy_constructible_v<Type> && std::is_copy_assignable_v<Type>;

	template<typename Type>
	concept Movable = std::is_move_constructible_v<Type> && std::is_move_assignable_v<Type>;

	template<typename Type>
	concept Comparable = requires(const Type& left, const Type& right)
	{
		left < right;
		left > right;
	};

	/** Avoid copy-constructing when comparing non-trivially copiable objects */
	template<typename Type>
	concept ComparableTrivial = Comparable<Type> &&
		                        std::is_trivially_copy_constructible_v<Type> &&
		                        sizeof(Type) <= kLargeDataSize;

	template<typename Type>
	concept ComparableNonTrivial = Comparable<Type> && 
	                               !std::is_trivially_copy_constructible_v<Type> ||
	                               sizeof(Type) > kLargeDataSize;

	template<typename Type>
	concept EnabledToString = requires(const Type& object) { object.ToString(); };

	template<typename Type>
	concept EnabledCout = requires(const Type& object) { std::cout << object; };
}