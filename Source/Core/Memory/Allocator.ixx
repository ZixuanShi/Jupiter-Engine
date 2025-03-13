// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <new>
#include <stdlib.h>
#include <cstring>
#include <initializer_list>
#include <memory>

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
        /** Allocates plain heap memory for <Type>*/
        static constexpr T* Allocate();
        static constexpr T* AllocateArray(size_t count);

        /** Allocates heap memory for <Type>, with initializing value */
        template<typename ...TArgs>
        static constexpr T* AllocateWithValue(TArgs&&... args);
        static constexpr T* AllocateArrayWithValues(size_t count, std::initializer_list<T>&& values);

        /** Deallocate memory for the passed in pointer */
        static constexpr void Deallocate(T* pPointer);
        static constexpr void DeallocateArray(T* pArray);

        template<typename ...TArgs>
        static constexpr void Construct(T* pPointer, TArgs&&... args);
        static constexpr void Destruct(T* pPointer);

        template<class TOther>
        constexpr bool operator==(const Allocator<TOther>&) const noexcept;

        template<class TOther>
        constexpr bool operator!=(const Allocator<TOther>&) const noexcept;
    };

    template<typename T>
    constexpr T* Allocator<T>::Allocate()
    {
        return new T;
    }

    template<typename T>
    constexpr T* Allocator<T>::AllocateArray(size_t count)
    {
        if (count == 0) 
        {
            return nullptr;
        }

        // Check for overflow
        JPT_ASSERT(count <= std::numeric_limits<size_t>::max() / sizeof(T));

        return new T[count];
    }

    template<typename T>
    template<typename ...TArgs>
    constexpr T* Allocator<T>::AllocateWithValue(TArgs&& ...args)
    {
        return new T(Forward<TArgs>(args)...);
    }

    template<typename T>
    constexpr T* Allocator<T>::AllocateArrayWithValues(size_t count, std::initializer_list<T>&& values)
    {
        T* pArray = new T[count];

        size_t i = 0;
        for (const T& element : values)
        {
            pArray[i] = Move(const_cast<T&>(element));
            ++i;
        }

        return pArray;
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
    template<typename ...TArgs>
    constexpr void Allocator<T>::Construct(T* pPointer, TArgs&& ...args)
    {
        std::construct_at(pPointer, Forward<TArgs>(args)...);
    }

    template<typename T>
    constexpr void Allocator<T>::Destruct(T* pPointer)
    {
        std::destroy_at(pPointer);
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