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
	/** Equivalent of std::move. Returns a rvalue of the object */
	template<Movable T>
	typename RemoveReference<T>::Type&& Move(T&& object)
	{
		using CastT = RemoveReference<T>::Type;

		static_assert(IsLValueReferenceType<T>, "jpt::move called on an lvalue");
		static_assert(!IsSameType<CastT&, const CastT&>, "jpt::move called on a const object");

		return static_cast<CastT&&>(object);
	}

	/** Equivalent of std::forward. Return a reference to an rvalue reference. */
	template<class T>
	T&& Forward(typename RemoveReference<T>::Type& obj)
	{
		return static_cast<T&&>(obj);
	}

	template<class T>
	T&& Forward(typename RemoveReference<T>::Type&& obj)
	{
		static_assert(!IsLValueReferenceType<T>, "Bad forward call");
		return static_cast<T&&>(obj);
	}

	/** Equivalent for std::swap. Swaps the value of two items */
	template<Movable T>
	void Swap(T& a, T& b)
	{
		T temp = jpt::Move(b);
		b = jpt::Move(a);
		a = jpt::Move(temp);
	}

	/** Equivalent for std::hash. Calculate the key's hash value */
	template<class KeyT>
	struct hash
	{
		/** Example: jpt::hash<jpt::string>()(key) */
		size_t operator()(const KeyT& key)
		{
			// TODO: Write my own std::hash
			return std::hash<KeyT>()(key);
		}
	};
}