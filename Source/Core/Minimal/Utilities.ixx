// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file contains minimum C++ programming utility functions.
// Without these functions we can't write a complete program in C++

module;

#include <functional>

export module jpt.Utilities;

import jpt.Concepts;
import jpt.TypeTraits;

export namespace jpt
{
	/** Returns a rvalue of the object */
	template<MoveConstructible T>
	constexpr TRemoveReference<T>&& Move(T&& object)
	{
		using TCast = TRemoveReference<T>;
		return static_cast<TCast&&>(object);
	}

	/** Returns LValue if obj is LValue, Returns RValue if obj is RValue */
	template<class T>
	constexpr T&& Forward(TRemoveReference<T>& obj)
	{
		return static_cast<T&&>(obj);
	}

	template<class T>
	constexpr T&& Forward(TRemoveReference<T>&& obj)
	{
		static_assert(!IsLValueRef<T>, "Bad forward call");
		return static_cast<T&&>(obj);
	}

	/** Equivalent for std::swap. Swaps the value of two items */
	template<Movable T>
	constexpr void Swap(T& a, T& b)
	{
		T temp = jpt::Move(b);
		b = jpt::Move(a);
		a = jpt::Move(temp);
	}

	/** @return		How many args of parameter pack. Works on Macro's __VA_ARGS__ too */
	template<typename ...Ts>
	constexpr size_t GetArgsCount(Ts&&...)
	{
		return sizeof...(Ts);
	}
	template<typename ...Ts>
	constexpr bool HasAnyArgs(Ts&&...)
	{
		return sizeof...(Ts) > 0;
	}
}