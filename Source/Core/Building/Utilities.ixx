// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file contains minimum C++ programming utility functions.
// Without these functions we can't write a complete program in C++

module;

#include <functional>

export module jpt.Utilities;

import jpt.Concepts;
import jpt.TypeDefs;

export namespace jpt
{
	/** Equivalent of std::move. Returns a rvalue of the object */
	template<Movable Type>
	typename RemoveReference<Type>::Type&& Move(Type&& object)
	{
		using CastType = typename RemoveReference<Type>::Type;

		static_assert(IsLValueReferenceType<Type>, "jpt::move called on an lvalue");
		static_assert(!jpt::IsSameType<CastType&, const CastType&>, "jpt::move called on a const object");

		return static_cast<CastType&&>(object);
	}

	/** Equivalent of std::forward. Return a reference to an rvalue reference. */
	template<typename T>
	T&& Forward(typename RemoveReference<T>::Type& obj)
	{
		return static_cast<T&&>(obj);
	}

	template<typename T>
	T&& Forward(typename RemoveReference<T>::Type&& obj)
	{
		static_assert(!IsLValueReferenceType<T>, "Bad forward call");
		return static_cast<T&&>(obj);
	}

	/** Equivalent for std::swap. Swaps the value of two items */
	template<Movable Type>
	void Swap(Type& a, Type& b)
	{
		Type temp = jpt::Move(b);
		b = jpt::Move(a);
		a = jpt::Move(temp);
	}

	/** Equivalent for std::hash. Calculate the key's hash value */
	template<class KeyType>
	struct hash
	{
		/** Example: jpt::hash<jpt::string>()(key) */
		size_t operator()(const KeyType& key)
		{
			// TODO: Write my own std::hash
			return std::hash<KeyType>()(key);
		}
	};
}