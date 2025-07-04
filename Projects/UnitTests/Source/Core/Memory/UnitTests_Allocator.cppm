// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Allocator;

import jpt.Allocator;
import jpt.TypeDefs;
import jpt.Concepts;
import jpt.Utilities;
import jpt.String;
import jpt.ToString;

template<jpt::Numeric TNum>
bool UnitTests_TAllocatorrivialType()
{
    // Single
    TNum* pSingle = jpt::Allocator<TNum>::Allocate();
    JPT_ENSURE(pSingle != nullptr);
    jpt::Allocator<TNum>::Deallocate(pSingle);

    // Single with value
    TNum* pSingleWithValue = jpt::Allocator<TNum>::AllocateWithValue(static_cast<TNum>(42));
    JPT_ENSURE(pSingleWithValue != nullptr);
    JPT_ENSURE(*pSingleWithValue == static_cast<TNum>(42));
    jpt::Allocator<TNum>::Deallocate(pSingleWithValue);

    // Array
    constexpr size_t kArraySize = 10'000;
    TNum* pArray = jpt::Allocator<TNum>::AllocateArray(kArraySize);
    JPT_ENSURE(pArray != nullptr);

    for (size_t i = 0; i < kArraySize; ++i)
    {
        pArray[i] = static_cast<TNum>(i);
    }
    for (size_t i = 0; i < kArraySize; ++i)
    {
        JPT_ENSURE(pArray[i] == static_cast<TNum>(i));
    }
    jpt::Allocator<TNum>::DeallocateArray(pArray);

    // Multi with value initializer list
    TNum* pMultiWithValue = jpt::Allocator<TNum>::AllocateArrayWithValues(10, { 0,1,2,3,4,5,6,7,8,9 });
    JPT_ENSURE(pMultiWithValue != nullptr);
    for (size_t i = 0; i < 10; ++i)
    {
        JPT_ENSURE(pMultiWithValue[i] == static_cast<TNum>(i));
    }
    jpt::Allocator<TNum>::Deallocate(pMultiWithValue);

    return true;
}

bool UnitTests_TAllocator_NonTrivial()
{
    // If uncomment the following JPT_LOGs, only one "Foo copy ctor called" should be printed

    class Foo
    {
    private:
        jpt::String m_str;

    public:
        Foo() 
        {
            //JPT_LOG("Foo default ctor called");
        }

        Foo(const jpt::String& str) 
            : m_str(str) 
        {
            //JPT_LOG("Foo String ctor called");
        }

        Foo(const Foo& other) 
            : m_str(other.m_str)
        {
            //JPT_LOG("Foo copy ctor called");
        }

        Foo(Foo&& other)
            : m_str(jpt::Move(other.m_str))
        {
            //JPT_LOG("Foo move ctor called");
        }

        Foo& operator=(const Foo& other) 
        { 
            //JPT_LOG("Foo copy operator= called");
            m_str = other.m_str;
            return *this; 
        }

        Foo& operator=(Foo&& other) 
        { 
            //JPT_LOG("Foo move operator= called");
            m_str = jpt::Move(other.m_str);
            return *this; 
        }

        ~Foo() 
        {
            //JPT_LOG("Foo dtor called");
        }

        jpt::String ToString() const 
        { 
            return "Foo" + m_str;
        }
    };

    jpt::Allocator<Foo> allocator;

    Foo* pFoo = allocator.Allocate();
    JPT_ENSURE(pFoo != nullptr);
    allocator.Deallocate(pFoo);

    Foo* pFooWithArgs = allocator.AllocateWithValue(jpt::String("Bar"));
    JPT_ENSURE(pFooWithArgs != nullptr);
    JPT_ENSURE(pFooWithArgs->ToString() == "FooBar");
    allocator.Deallocate(pFooWithArgs);

    Foo* pFooArray = allocator.AllocateArrayWithValues(10, { Foo("0"), Foo("1"), Foo("2"), Foo("3"), Foo("4"), Foo("5"), Foo("6"), Foo("7"),Foo("8"), Foo("9") });
    JPT_ENSURE(pFooArray != nullptr);
    for (size_t i = 0; i < 10; ++i)
    {
        JPT_ENSURE(pFooArray[i].ToString() == "Foo" + jpt::ToString(i));
    }
    allocator.DeallocateArray(pFooArray);

    Foo* pConstructedFoo = allocator.Allocate();
    allocator.Construct(pConstructedFoo, Foo("Bar"));
    JPT_ENSURE(pConstructedFoo != nullptr);
    JPT_ENSURE(pConstructedFoo->ToString() == "FooBar");
    allocator.Destruct(pConstructedFoo);

    Foo anotherFoo = jpt::String("Baz");
    allocator.Construct(pConstructedFoo, anotherFoo);
    JPT_ENSURE(pConstructedFoo != nullptr);
    JPT_ENSURE(pConstructedFoo->ToString() == "FooBaz");
    allocator.Destruct(pConstructedFoo);

    allocator.Construct(pConstructedFoo, jpt::Move(anotherFoo));
    JPT_ENSURE(pConstructedFoo != nullptr);
    JPT_ENSURE(pConstructedFoo->ToString() == "FooBaz");
    allocator.Destruct(pConstructedFoo);

    allocator.Deallocate(pConstructedFoo);

    return true;
}

export bool RunUnitTests_Allocator()
{
    JPT_ENSURE(UnitTests_TAllocatorrivialType<uint8>());
    JPT_ENSURE(UnitTests_TAllocatorrivialType<int32>());
    JPT_ENSURE(UnitTests_TAllocatorrivialType<float>());
    JPT_ENSURE(UnitTests_TAllocatorrivialType<double>());

    JPT_ENSURE(UnitTests_TAllocator_NonTrivial());

    return true;
}