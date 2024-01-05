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
	/** Equivalent for std::move. Returns a rvalue of the object */
	template<Movable Type>
	typename RemoveReference<Type>::Type&& move(Type&& object)
	{
		using CastType = typename RemoveReference<Type>::Type;

		static_assert(IsLValueReferenceType<Type>::Value, "jpt::move called on an lvalue");
		static_assert(!jpt::IsSameType<CastType&, const CastType&>::Value, "jpt::move called on a const object");

		return static_cast<CastType&&>(object);
	}

	/** Equivalent for std::swap. Swaps the value of two items */
	template<Movable Type>
	void swap(Type& a, Type& b)
	{
		Type temp = jpt::move(b);
		b = jpt::move(a);
		a = jpt::move(temp);
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