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
		static Type* Allocate(size_t count = 1);
		static Type* AllocateArray(size_t count = 1);

		/** Allocates heap memory for one <Type>, with initializing value */
		static Type* AllocateWithValue(const Type& data);

		/** Allocates heap memory for desired amount of memory for <Type> with init values
			@param count		How many <Type> objects to allocate 
			@param values		An pointer to an array for each allocated objects initializing value. */
		static Type* AllocateMultiWithValue(size_t count, const Type* pValues);
		static Type* AllocateMultiWithValue(size_t count, const std::initializer_list<Type>& pValues);

		/** Deallocate memory for the passed in pointer */
		static void Deallocate(Type* pPointer);
		static void DeallocateArray(Type* pArray);

		static void Construct(Type* pPointer, const Type& object);
		static void Destruct(Type* pPointer);

		template<class OtherType>
		bool operator==(const Allocator<OtherType>&) const noexcept;

		template<class OtherType>
		bool operator!=(const Allocator<OtherType>&) const noexcept;
	};

	template<typename Type>
	Type* Allocator<Type>::Allocate(size_t count /* = 1 */ )
	{
		return static_cast<Type*>(::operator new(count * sizeof(Type)));
	}

	template<typename Type>
	Type* Allocator<Type>::AllocateArray(size_t count /* = 1 */)
	{
		return static_cast<Type*>(::operator new[](count * sizeof(Type)));
	}

	template<typename Type>
	Type* Allocator<Type>::AllocateWithValue(const Type& data)
	{
		Type* pPointer = static_cast<Type*>(::operator new(sizeof(Type)));
		*pPointer = data;
		return pPointer;
	}

	template<typename Type>
	Type* Allocator<Type>::AllocateMultiWithValue(size_t count, const Type* pValues)
	{
		Type* pArray = static_cast<Type*>(::operator new(count * sizeof(Type)));
		std::memcpy(pArray, pValues, count * sizeof(Type));
		return pArray;
	}

	template<typename Type>
	Type* Allocator<Type>::AllocateMultiWithValue(size_t count, const std::initializer_list<Type>& pValues)
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
	void Allocator<Type>::Deallocate(Type* pPointer)
	{
		::operator delete(pPointer);
	}

	template<typename Type>
	void Allocator<Type>::DeallocateArray(Type* pArray)
	{
		::operator delete[](pArray);
	}

	template<typename Type>
	void Allocator<Type>::Construct(Type* pPointer, const Type& object)
	{
		new(pPointer) Type(object);
	}

	template<typename Type>
	void Allocator<Type>::Destruct(Type* pPointer)
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