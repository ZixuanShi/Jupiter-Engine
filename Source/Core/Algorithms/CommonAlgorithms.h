#pragma once

namespace jpt
{
	template<class Type>
	typename RemoveReference<Type>::Type&& move(Type&& object)
	{
		using CastType = typename RemoveReference<Type>::Type;

		// Validate that we're not being passed an rvalue or a const object - the former is redundant, the latter is almost certainly a mistake
		static_assert(IsLValueReferenceType<Type>::Value, "MoveTemp called on an rvalue");
		static_assert(!IsSameType<CastType&, const CastType&>::Value, "MoveTemp called on a const object");

		return static_cast<CastType&&>(object);
	}

	template<class Type>
	void swap(Type& a, Type& b)
	{
		Type temp = jpt::move(a);
		a = jpt::move(b);
		b = jpt::move(temp);
	}
}