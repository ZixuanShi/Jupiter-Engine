// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <stdlib.h>
#include <cstring>
#include <initializer_list>

export module jpt.Allocator;

import jpt.Utilities;

export namespace jpt
{
	/** Allocate and deallocate memory for objects. 
		@see https://www.codeproject.com/Articles/4795/C-Standard-Allocator-An-Introduction-and-Implement
		@see https://learn.microsoft.com/en-us/cpp/standard-library/allocators?view=msvc-170#writing-your-own-allocator-c11 
		@see https://www.geeksforgeeks.org/stdallocator-in-cpp-with-examples */
	template<typename Type>
	class Allocator
	{
	public:
		Allocator() = default;

		template<class OtherType>
		Allocator(const Allocator<OtherType>& other) noexcept {}

	public:
		/** Allocates plain heap memory for desired amount of memory for <Type>
			@param count		How many <Type> objects to allocate */
		Type* Allocate(size_t count = 0) const;

		/** Allocates heap memory for one <Type>, with initializing value */
		Type* AllocateWithValue(Type data) const;

		/** Allocates heap memory for desired amount of memory for <Type> with init values
			@param count		How many <Type> objects to allocate 
			@param values		An pointer to an array for each allocated objects initializing value. */
		Type* AllocateMultiWithValue(size_t count, Type* pValues) const;
		Type* AllocateMultiWithValue(size_t count, const std::initializer_list<Type>& pValues) const;
		Type* AllocateMultiWithValue(size_t count, std::initializer_list<Type>&& pValues) const;

		/** Deallocate memory for the passed in pointer */
		void Deallocate(Type* pPointer) const;

		void Construct(Type* pPointer, const Type& object) const;
		void Destroy(Type* pPointer) const;

		template<class OtherType>
		bool operator==(const Allocator<OtherType>&) const noexcept;

		template<class OtherType>
		bool operator!=(const Allocator<OtherType>&) const noexcept;
	};

	template<typename Type>
	Type* Allocator<Type>::Allocate(size_t count /* = 0 */ ) const
	{
		return static_cast<Type*>(::operator new(count * sizeof(Type)));
	}

	template<typename Type>
	Type* Allocator<Type>::AllocateWithValue(Type data) const
	{
		Type* pPointer = static_cast<Type*>(::operator new(sizeof(Type)));
		*pPointer = data;
		return pPointer;
	}

	template<typename Type>
	Type* Allocator<Type>::AllocateMultiWithValue(size_t count, Type* pValues) const
	{
		Type* pArray = static_cast<Type*>(::operator new(count * sizeof(Type)));
		std::memcpy(pArray, pValues, count * sizeof(Type));
		return pArray;
	}

	template<typename Type>
	Type* Allocator<Type>::AllocateMultiWithValue(size_t count, const std::initializer_list<Type>& pValues) const
	{
		Type* pArray = static_cast<Type*>(::operator new(count * sizeof(Type)));

		size_t i = 0;
		for (auto itr = pValues.begin(); itr != pValues.end(); ++itr)
		{
			pArray[i] = *itr;
			++i;
		}

		return pArray;
	}

	template<typename Type>
	Type* Allocator<Type>::AllocateMultiWithValue(size_t count, std::initializer_list<Type>&& pValues) const
	{
		Type* pArray = static_cast<Type*>(::operator new(count * sizeof(Type)));

		size_t i = 0;
		for (auto itr = pValues.begin(); itr != pValues.end(); ++itr)
		{
			pArray[i] = move(*itr);
			++i;
		}

		return pArray;
	}

	template<typename Type>
	void Allocator<Type>::Deallocate(Type* pPointer) const
	{
		::operator delete(pPointer);
	}

	template<typename Type>
	void Allocator<Type>::Construct(Type* pPointer, const Type& object) const
	{
		new(pPointer) Type(object);
	}

	template<typename Type>
	void Allocator<Type>::Destroy(Type* pPointer) const
	{
		pPointer->~Type();
	}

	template<typename Type>
	template<class OtherType>
	bool Allocator<Type>::operator==(const Allocator<OtherType>&) const noexcept
	{
		return true;
	}

	template<typename Type>
	template<class OtherType>
	bool Allocator<Type>::operator!=(const Allocator<OtherType>&) const noexcept
	{
		return false;
	}
}