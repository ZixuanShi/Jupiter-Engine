// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	/** Equivalent for std::move. Returns a rvalue of the object */
	template<class Type>
	typename RemoveReference<Type>::Type&& move(Type&& object)
	{
		using CastType = typename RemoveReference<Type>::Type;
		return static_cast<CastType&&>(object);
	}

	/** Equivalent for std::swap. Swaps the value of two items */
	template<class Type>
	void swap(Type& a, Type& b)
	{
		Type temp = jpt::move(a);
		a = jpt::move(b);
		b = jpt::move(temp);
	}

	/** Equivalent for std::hash. Calculate the key's hash value */
	template<class KeyType>
	struct hash
	{
		/** Example: jpt::hash<jpt::string>()(key) */
		size_t operator()(const KeyType& key)
		{
			// TODO: Write my own
			return std::hash<KeyType>()(key);
		}
	};
}