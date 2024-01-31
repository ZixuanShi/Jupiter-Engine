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
	template<typename T>
	class Allocator
	{
	public:
		constexpr Allocator() = default;

		template<class TOther>
		constexpr Allocator(const Allocator<TOther>& other) noexcept {}

	public:
		/** Allocates plain heap memory for desired amount of memory for <Type>
			@param count		How many <Type> objects to allocate */
		static constexpr T* Allocate(size_t count = 1);
		static constexpr T* AllocateArray(size_t count, const std::initializer_list<T>& values = {});

		/** Allocates heap memory for one <Type>, with initializing value */
		static constexpr T* AllocateWithValue(const T& data);
		static constexpr T* AllocateWithValue(T&& data);

		/** Deallocate memory for the passed in pointer */
		static constexpr void Deallocate(T* pPointer);
		static constexpr void DeallocateArray(T* pArray);

		static constexpr void Construct(T* pPointer, const T& object);
		static constexpr void Construct(T* pPointer, T&& object);
		template<typename ...TArgs>
		static constexpr void Emplace(T* pPointer, TArgs&&... args);
		static constexpr void Destruct(T* pPointer);

		template<class TOther>
		constexpr bool operator==(const Allocator<TOther>&) const noexcept;

		template<class TOther>
		constexpr bool operator!=(const Allocator<TOther>&) const noexcept;
	};

	template<typename T>
	constexpr T* Allocator<T>::Allocate(size_t count /* = 1 */ )
	{
		return new T[count];
	}

	template<typename T>
	constexpr T* Allocator<T>::AllocateArray(size_t count, const std::initializer_list<T>& values/* = {}*/)
	{
		T* pArray = new T[count];

		size_t i = 0;
		for (const T& element : values)
		{
			pArray[i] = element;
			++i;
		}

		return pArray;
	}

	template<typename T>
	constexpr T* Allocator<T>::AllocateWithValue(const T& data)
	{
		return new T(data);
	}

	template<typename T>
	constexpr T* Allocator<T>::AllocateWithValue(T&& data)
	{
		return new T(Move(data));
	}

	template<typename T>
	constexpr void Allocator<T>::Deallocate(T* pPointer)
	{
		delete pPointer;
	}

	template<typename T>
	constexpr void Allocator<T>::DeallocateArray(T* pArray)
	{
		delete[] pArray;
	}

	template<typename T>
	constexpr void Allocator<T>::Construct(T* pPointer, const T& object)
	{
		new(pPointer) T(object);
	}

	template<typename T>
	constexpr void Allocator<T>::Construct(T* pPointer, T&& object)
	{
		new(pPointer) T(Move(object));
	}

	template<typename T>
	template<typename ...TArgs>
	constexpr void Allocator<T>::Emplace(T* pPointer, TArgs && ...args)
	{
		new(pPointer) T(Forward<TArgs>(args)...);
	}

	template<typename T>
	constexpr void Allocator<T>::Destruct(T* pPointer)
	{
		pPointer->~T();
	}

	template<typename T>
	template<class TOtherype>
	constexpr bool Allocator<T>::operator==(const Allocator<TOtherype>&) const noexcept
	{
		return true;
	}

	template<typename T>
	template<class TOtherype>
	constexpr bool Allocator<T>::operator!=(const Allocator<TOtherype>&) const noexcept
	{
		return false;
	}
}