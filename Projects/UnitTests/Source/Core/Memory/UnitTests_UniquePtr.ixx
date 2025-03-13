// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_UniquePtr;

import jpt.Allocator;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.UniquePtr;
import jpt.ToString;
import jpt.String;

bool UnitTests_UniquePtr_Char()
{
    auto deleter = [](char* pCharPtr)
        {
            //JPT_LOG("Deleted a char %c", *pCharPtr);
            delete pCharPtr;
        };

    auto deleter2 = [](char* pCharPtr)
        {
            //JPT_LOG("Another Deleted a char %c", *pCharPtr);
            delete pCharPtr;
        };

    jpt::UniquePtr<char> uniqueChar = jpt::MakeUnique<char>('C');
    JPT_ENSURE(uniqueChar.IsValid());
    JPT_ENSURE(uniqueChar);
    JPT_ENSURE(*uniqueChar == 'C');

    uniqueChar.Reset(new char('D'));
    JPT_ENSURE(*uniqueChar == 'D');

    char* c = uniqueChar.Release();
    JPT_ENSURE(!uniqueChar);
    JPT_ENSURE(!uniqueChar.IsValid());
    JPT_ENSURE(*c == 'D');
    delete c;

    jpt::UniquePtr<char, decltype(deleter)> customDeleterUniquePtr = jpt::UniquePtr<char, decltype(deleter)>(new char('C'), deleter);
    JPT_ENSURE(customDeleterUniquePtr.IsValid());
    JPT_ENSURE(customDeleterUniquePtr);
    JPT_ENSURE(*customDeleterUniquePtr == 'C');

    customDeleterUniquePtr.Reset(new char('D'));
    JPT_ENSURE(*customDeleterUniquePtr == 'D');

    jpt::UniquePtr<char, decltype(deleter2)> otherCustomDeleterUniquePtr = jpt::UniquePtr<char, decltype(deleter2)>(new char('D'), deleter2);

    otherCustomDeleterUniquePtr.GetDeleter()(customDeleterUniquePtr.Release());
    JPT_ENSURE(!customDeleterUniquePtr);
    JPT_ENSURE(!customDeleterUniquePtr.IsValid());
    JPT_ENSURE(*otherCustomDeleterUniquePtr == 'D');

    jpt::UniquePtr<char, decltype(deleter)> source = jpt::UniquePtr<char, decltype(deleter)>(new char('A'), deleter);
    jpt::UniquePtr<char, decltype(deleter)> other  = jpt::UniquePtr<char, decltype(deleter)>(new char('B'), deleter);
    source = jpt::Move(other);
    JPT_ENSURE(*source == 'B');

    jpt::UniquePtr<char[]> charArray(jpt::Allocator<char>::AllocateArrayWithValues(10, {'A', 'B', 'C', 'A'}));
    //JPT_LOG(charArray.Get()[0]);
    //JPT_LOG(charArray.Get()[1]);
    //JPT_LOG(charArray.Get()[2]);
    //JPT_LOG(charArray.Get()[3]);
    //JPT_LOG(charArray.Get()[4]);
    //JPT_LOG(charArray.Get()[5]);

    return true;
}

bool UnitTests_UniquePtr_Int()
{
    auto deleter = [](int32* pint32Ptr)
        {
            //JPT_LOG("Deleted a int32 %d", *pint32Ptr);
            delete pint32Ptr;
        };

    auto deleter2 = [](int32* pint32Ptr)
        {
            //JPT_LOG("Another Deleted a int32 %d", *pint32Ptr);
            delete pint32Ptr;
        };

    jpt::UniquePtr<int32> uniqueint32 = jpt::MakeUnique<int32>(42);
    JPT_ENSURE(uniqueint32.IsValid());
    JPT_ENSURE(uniqueint32);
    JPT_ENSURE(*uniqueint32 == 42);

    uniqueint32.Reset(new int32(42));
    JPT_ENSURE(*uniqueint32 == 42);

    int32* c = uniqueint32.Release();
    JPT_ENSURE(!uniqueint32);
    JPT_ENSURE(!uniqueint32.IsValid());
    JPT_ENSURE(*c == 42);
    delete c;

    jpt::UniquePtr<int32, decltype(deleter)> customDeleterUniquePtr = jpt::UniquePtr<int32, decltype(deleter)>(new int32(42), deleter);
    JPT_ENSURE(customDeleterUniquePtr.IsValid());
    JPT_ENSURE(customDeleterUniquePtr);
    JPT_ENSURE(*customDeleterUniquePtr == 42);

    customDeleterUniquePtr.Reset(new int32(42));
    JPT_ENSURE(*customDeleterUniquePtr == 42);

    jpt::UniquePtr<int32, decltype(deleter2)> otherCustomDeleterUniquePtr = jpt::UniquePtr<int32, decltype(deleter2)>(new int32(42), deleter2);

    otherCustomDeleterUniquePtr.GetDeleter()(customDeleterUniquePtr.Release());
    JPT_ENSURE(!customDeleterUniquePtr);
    JPT_ENSURE(!customDeleterUniquePtr.IsValid());
    JPT_ENSURE(*otherCustomDeleterUniquePtr == 42);

    jpt::UniquePtr<int32, decltype(deleter)> source = jpt::UniquePtr<int32, decltype(deleter)>(new int32(105), deleter);
    jpt::UniquePtr<int32, decltype(deleter)> other = jpt::UniquePtr<int32, decltype(deleter)>(new int32(101), deleter);
    source = jpt::Move(other);

    jpt::UniquePtr<int32[]> int32Array(jpt::Allocator<int32>::AllocateArrayWithValues(10, { 1, 2, 3, 4 }));
    //JPT_LOG(int32Array.Get()[0]);
    //JPT_LOG(int32Array.Get()[1]);
    //JPT_LOG(int32Array.Get()[2]);
    //JPT_LOG(int32Array.Get()[3]);
    //JPT_LOG(int32Array.Get()[4]);
    //JPT_LOG(int32Array.Get()[5]);

    return true;
}

bool UnitTests_UniquePtr_String()
{
    using String = jpt::String;

    auto deleter = [](String* pStringPtr)
        {
            //JPT_LOG("Deleted a String %s", pStringPtr->ConstBuffer());
            delete pStringPtr;
        };

    auto deleter2 = [](String* pStringPtr)
        {
            //JPT_LOG("Another Deleted a String %s", pStringPtr->ConstBuffer());
            delete pStringPtr;
        };

    jpt::UniquePtr<String> uniqueString = jpt::MakeUnique<String>("Jupiter");
    JPT_ENSURE(uniqueString.IsValid());
    JPT_ENSURE(uniqueString);
    JPT_ENSURE(*uniqueString == "Jupiter");

    uniqueString.Reset(new String("Engine"));
    JPT_ENSURE(*uniqueString == "Engine");

    String* c = uniqueString.Release();
    JPT_ENSURE(!uniqueString);
    JPT_ENSURE(!uniqueString.IsValid());
    JPT_ENSURE(*c == "Engine");
    delete c;

    jpt::UniquePtr<String, decltype(deleter)> customDeleterUniquePtr = jpt::UniquePtr<String, decltype(deleter)>(new String("Jupiter"), deleter);
    JPT_ENSURE(customDeleterUniquePtr.IsValid());
    JPT_ENSURE(customDeleterUniquePtr);
    JPT_ENSURE(*customDeleterUniquePtr == "Jupiter");

    customDeleterUniquePtr.Reset(new String("Engine"));
    JPT_ENSURE(*customDeleterUniquePtr == "Engine");

    jpt::UniquePtr<String, decltype(deleter2)> otherCustomDeleterUniquePtr = jpt::UniquePtr<String, decltype(deleter2)>(new String("Engine"), deleter2);

    otherCustomDeleterUniquePtr.GetDeleter()(customDeleterUniquePtr.Release());
    JPT_ENSURE(!customDeleterUniquePtr);
    JPT_ENSURE(!customDeleterUniquePtr.IsValid());
    JPT_ENSURE(*otherCustomDeleterUniquePtr == "Engine");

    jpt::UniquePtr<String, decltype(deleter)> source = jpt::UniquePtr<String, decltype(deleter)>(new String("105"), deleter);
    jpt::UniquePtr<String, decltype(deleter)> other = jpt::UniquePtr<String, decltype(deleter)>(new String("101"), deleter);
    source = jpt::Move(other);

    return true;
}

bool UnitTests_UniquePtr_Class()
{
    struct Foo
    {
        int32 m_left = 0;
        char m_right = '0';

        Foo() = default;
        Foo(int32 left, char right) : m_left(left), m_right(right) {}

        jpt::String ToString() const 
        {
            jpt::String result;
            result += jpt::ToString(m_left);
            result += ' ';
            result += jpt::ToString(m_right);
            return result;
        }
    };

    auto deleter = [](Foo* pFooPtr)
        {
            //JPT_LOG("Deleted a Foo %s", pFooPtr->ToString().ConstBuffer());
            delete pFooPtr;
        };

    auto deleter2 = [](Foo* pFooPtr)
        {
            //JPT_LOG("Another Deleted a Foo %s", pFooPtr->ToString().ConstBuffer());
            delete pFooPtr;
        };

    jpt::UniquePtr<Foo> uniqueFoo = jpt::MakeUnique<Foo>(42, 'C');
    JPT_ENSURE(uniqueFoo.IsValid());
    JPT_ENSURE(uniqueFoo);
    JPT_ENSURE(uniqueFoo->m_left == 42);
    JPT_ENSURE(uniqueFoo->m_right == 'C');

    uniqueFoo.Reset(new Foo(41, 'D'));
    JPT_ENSURE(uniqueFoo->m_left == 41);
    JPT_ENSURE(uniqueFoo->m_right == 'D');

    Foo* c = uniqueFoo.Release();
    JPT_ENSURE(!uniqueFoo);
    JPT_ENSURE(!uniqueFoo.IsValid());
    JPT_ENSURE(c->m_left == 41);
    JPT_ENSURE(c->m_right == 'D');
    delete c;

    jpt::UniquePtr<Foo, decltype(deleter)> customDeleterUniquePtr = jpt::UniquePtr<Foo, decltype(deleter)>(new Foo(42, 'C'), deleter);
    JPT_ENSURE(customDeleterUniquePtr.IsValid());
    JPT_ENSURE(customDeleterUniquePtr);
    JPT_ENSURE(customDeleterUniquePtr->m_left == 42);
    JPT_ENSURE(customDeleterUniquePtr->m_right == 'C');

    customDeleterUniquePtr.Reset(new Foo(42, 'C'));
    JPT_ENSURE(customDeleterUniquePtr->m_left == 42);
    JPT_ENSURE(customDeleterUniquePtr->m_right == 'C');

    jpt::UniquePtr<Foo, decltype(deleter2)> otherCustomDeleterUniquePtr = jpt::UniquePtr<Foo, decltype(deleter2)>(new Foo(42, 'C'), deleter2);

    otherCustomDeleterUniquePtr.GetDeleter()(customDeleterUniquePtr.Release());
    JPT_ENSURE(!customDeleterUniquePtr);
    JPT_ENSURE(!customDeleterUniquePtr.IsValid());
    JPT_ENSURE(otherCustomDeleterUniquePtr->m_left == 42);
    JPT_ENSURE(otherCustomDeleterUniquePtr->m_right == 'C');

    jpt::UniquePtr<Foo, decltype(deleter)> source = jpt::UniquePtr<Foo, decltype(deleter)>(new Foo(42, 'a'), deleter);
    jpt::UniquePtr<Foo, decltype(deleter)> other = jpt::UniquePtr<Foo, decltype(deleter)>(new Foo(43, 'c'), deleter);
    source = jpt::Move(other);

    jpt::UniquePtr<Foo[]> FooArray(jpt::Allocator<Foo>::AllocateArrayWithValues(10, { {1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'} }));
    //JPT_LOG(FooArray.Get()[0]);
    //JPT_LOG(FooArray.Get()[1]);
    //JPT_LOG(FooArray.Get()[2]);
    //JPT_LOG(FooArray.Get()[3]);
    //JPT_LOG(FooArray.Get()[4]);
    //JPT_LOG(FooArray.Get()[5]);

    return true;
}

export bool RunUnitTests_UniquePtr()
{
    JPT_ENSURE(UnitTests_UniquePtr_Char());
    JPT_ENSURE(UnitTests_UniquePtr_Int());
    JPT_ENSURE(UnitTests_UniquePtr_String());
    JPT_ENSURE(UnitTests_UniquePtr_Class());

    return true;
}