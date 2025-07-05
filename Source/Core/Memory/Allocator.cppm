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
    /** Allocate and deallocate memory for objects. */
    template<typename T>
    class Allocator
    {
    public:
        /** Allocates plain heap memory for <Type>*/
        template<typename ...TArgs> 
        static constexpr T* New(TArgs&&... args);

        template<typename ...TArgs> 
        static constexpr T* NewArray(size_t count, TArgs&&... args);

        /** Deallocate memory for the passed in pointer */
        static constexpr void Delete(T* pPointer);
        static constexpr void DeleteArray(T* pArray);

        template<typename ...TArgs>
        static constexpr void Construct(T* pPointer, TArgs&&... args);
        static constexpr void Destruct(T* pPointer);
    };

    template<typename T>
    template<typename ...TArgs>
    constexpr T* Allocator<T>::New(TArgs&&... args)
    {
        return new T(Forward<TArgs>(args)...);
    }

    template<typename T>
    template<typename ...TArgs>
    constexpr T* Allocator<T>::NewArray(size_t count, TArgs&&... args)
    {
        if (count == 0) 
        {
            return nullptr;
        }

        // Check for overflow
        JPT_ASSERT(count <= std::numeric_limits<size_t>::max() / sizeof(T));

        return new T[count]{ static_cast<T>(Forward<TArgs>(args))... };
    }

    template<typename T>
    constexpr void Allocator<T>::Delete(T* pPointer)
    {
        delete pPointer;
    }

    template<typename T>
    constexpr void Allocator<T>::DeleteArray(T* pArray)
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
}