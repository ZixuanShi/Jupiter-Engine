// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <type_traits>
#include <iostream>

export module jpt.CoreConcepts;

export namespace jpt
{
	template<typename Type>
	concept Numeric = std::is_arithmetic_v<Type>;

	template<typename Type>
	concept Integral = std::is_integral_v<Type>;

	template<typename Type>
	concept Floating = std::is_floating_point_v<Type>;

	template<typename Type>
	concept Copiable = std::is_copy_constructible_v<Type> && std::is_copy_assignable_v<Type>;

	template<typename Type>
	concept Movable = std::is_move_constructible_v<Type> && std::is_move_assignable_v<Type>;

	template<typename Type>
	concept Comparable = requires(const Type& left, const Type& right)
	{
		left < right;
		left > right;
		left <= right;
		left >= right;
	};

	template<typename Type>
	concept EnabledToString = requires(const Type& object) { object.ToString(); };

	template<typename Type>
	concept EnabledCout = requires(const Type& object) { std::cout << object; };
}