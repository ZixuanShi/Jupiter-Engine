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
            JPT_DELETE(pCharPtr);
        };

    auto deleter2 = [](char* pCharPtr)
        {
            //JPT_LOG("Another Deleted a char %c", *pCharPtr);
            JPT_DELETE(pCharPtr);
        };

    jpt::UniquePtr<char> uniqueChar = jpt::MakeUnique<char>('C');
    JPT_ENSURE(uniqueChar.IsValid());
    JPT_ENSURE(uniqueChar);
    JPT_ENSURE(*uniqueChar == 'C');

    uniqueChar.Reset(jpt::Allocator<char>::New('D'));
    JPT_ENSURE(*uniqueChar == 'D');

    char* c = uniqueChar.Release();
    JPT_ENSURE(!uniqueChar);
    JPT_ENSURE(!uniqueChar.IsValid());
    JPT_ENSURE(*c == 'D');
    JPT_DELETE(c);

    jpt::UniquePtr<char, decltype(deleter)> customDeleterUniquePtr = jpt::UniquePtr<char, decltype(deleter)>(jpt::Allocator<char>::New('C'), deleter);
    JPT_ENSURE(customDeleterUniquePtr.IsValid());
    JPT_ENSURE(customDeleterUniquePtr);
    JPT_ENSURE(*customDeleterUniquePtr == 'C');

    customDeleterUniquePtr.Reset(jpt::Allocator<char>::New('D'));
    JPT_ENSURE(*customDeleterUniquePtr == 'D');

    jpt::UniquePtr<char, decltype(deleter2)> otherCustomDeleterUniquePtr = jpt::UniquePtr<char, decltype(deleter2)>(jpt::Allocator<char>::New('D'), deleter2);

    otherCustomDeleterUniquePtr.GetDeleter()(customDeleterUniquePtr.Release());
    JPT_ENSURE(!customDeleterUniquePtr);
    JPT_ENSURE(!customDeleterUniquePtr.IsValid());
    JPT_ENSURE(*otherCustomDeleterUniquePtr == 'D');

    jpt::UniquePtr<char, decltype(deleter)> source = jpt::UniquePtr<char, decltype(deleter)>(jpt::Allocator<char>::New('A'), deleter);
    jpt::UniquePtr<char, decltype(deleter)> other  = jpt::UniquePtr<char, decltype(deleter)>(jpt::Allocator<char>::New('B'), deleter);
    source = jpt::Move(other);
    JPT_ENSURE(*source == 'B');

    jpt::UniquePtr<char[]> charArray(jpt::Allocator<char>::NewArray(10, 'A', 'B', 'C', 'A'));
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
            JPT_DELETE(pint32Ptr);
        };

    auto deleter2 = [](int32* pint32Ptr)
        {
            //JPT_LOG("Another Deleted a int32 %d", *pint32Ptr);
            JPT_DELETE(pint32Ptr);
        };

    jpt::UniquePtr<int32> uniqueint32 = jpt::MakeUnique<int32>(42);
    JPT_ENSURE(uniqueint32.IsValid());
    JPT_ENSURE(uniqueint32);
    JPT_ENSURE(*uniqueint32 == 42);

    uniqueint32.Reset(jpt::Allocator<int32>::New(42));
    JPT_ENSURE(*uniqueint32 == 42);

    int32* c = uniqueint32.Release();
    JPT_ENSURE(!uniqueint32);
    JPT_ENSURE(!uniqueint32.IsValid());
    JPT_ENSURE(*c == 42);
    JPT_DELETE(c);

    jpt::UniquePtr<int32, decltype(deleter)> customDeleterUniquePtr = jpt::UniquePtr<int32, decltype(deleter)>(jpt::Allocator<int32>::New(42), deleter);
    JPT_ENSURE(customDeleterUniquePtr.IsValid());
    JPT_ENSURE(customDeleterUniquePtr);
    JPT_ENSURE(*customDeleterUniquePtr == 42);

    customDeleterUniquePtr.Reset(jpt::Allocator<int32>::New(42));
    JPT_ENSURE(*customDeleterUniquePtr == 42);

    jpt::UniquePtr<int32, decltype(deleter2)> otherCustomDeleterUniquePtr = jpt::UniquePtr<int32, decltype(deleter2)>(jpt::Allocator<int32>::New(42), deleter2);

    otherCustomDeleterUniquePtr.GetDeleter()(customDeleterUniquePtr.Release());
    JPT_ENSURE(!customDeleterUniquePtr);
    JPT_ENSURE(!customDeleterUniquePtr.IsValid());
    JPT_ENSURE(*otherCustomDeleterUniquePtr == 42);

    jpt::UniquePtr<int32, decltype(deleter)> source = jpt::UniquePtr<int32, decltype(deleter)>(jpt::Allocator<int32>::New(105), deleter);
    jpt::UniquePtr<int32, decltype(deleter)> other = jpt::UniquePtr<int32, decltype(deleter)>(jpt::Allocator<int32>::New(101), deleter);
    source = jpt::Move(other);

    jpt::UniquePtr<int32[]> int32Array(jpt::Allocator<int32>::NewArray(10, 1, 2, 3, 4));
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
            JPT_DELETE(pStringPtr);
        };

    auto deleter2 = [](String* pStringPtr)
        {
            //JPT_LOG("Another Deleted a String %s", pStringPtr->ConstBuffer());
            JPT_DELETE(pStringPtr);
        };

    jpt::UniquePtr<String> uniqueString = jpt::MakeUnique<String>("Jupiter");
    JPT_ENSURE(uniqueString.IsValid());
    JPT_ENSURE(uniqueString);
    JPT_ENSURE(*uniqueString == "Jupiter");

    uniqueString.Reset(jpt::Allocator<String>::New("Engine"));
    JPT_ENSURE(*uniqueString == "Engine");

    String* c = uniqueString.Release();
    JPT_ENSURE(!uniqueString);
    JPT_ENSURE(!uniqueString.IsValid());
    JPT_ENSURE(*c == "Engine");
    JPT_DELETE(c);

    jpt::UniquePtr<String, decltype(deleter)> customDeleterUniquePtr = jpt::UniquePtr<String, decltype(deleter)>(jpt::Allocator<String>::New("Jupiter"), deleter);
    JPT_ENSURE(customDeleterUniquePtr.IsValid());
    JPT_ENSURE(customDeleterUniquePtr);
    JPT_ENSURE(*customDeleterUniquePtr == "Jupiter");

    customDeleterUniquePtr.Reset(jpt::Allocator<String>::New("Engine"));
    JPT_ENSURE(*customDeleterUniquePtr == "Engine");

    jpt::UniquePtr<String, decltype(deleter2)> otherCustomDeleterUniquePtr = jpt::UniquePtr<String, decltype(deleter2)>(jpt::Allocator<String>::New("Engine"), deleter2);

    otherCustomDeleterUniquePtr.GetDeleter()(customDeleterUniquePtr.Release());
    JPT_ENSURE(!customDeleterUniquePtr);
    JPT_ENSURE(!customDeleterUniquePtr.IsValid());
    JPT_ENSURE(*otherCustomDeleterUniquePtr == "Engine");

    jpt::UniquePtr<String, decltype(deleter)> source = jpt::UniquePtr<String, decltype(deleter)>(jpt::Allocator<String>::New("105"), deleter);
    jpt::UniquePtr<String, decltype(deleter)> other = jpt::UniquePtr<String, decltype(deleter)>(jpt::Allocator<String>::New("101"), deleter);
    source = jpt::Move(other);

    return true;
}

struct Foo
{
    int32 m_left = 0;
    char m_right = '0';

    Foo() = default;
    Foo(int32 left, char right) : m_left(left), m_right(right) {}

    bool operator==(const Foo& other) const noexcept
    {
        return m_left == other.m_left && m_right == other.m_right;
    }
};

namespace jpt
{
    constexpr jpt::String ToString(const Foo& foo)
    {
        return jpt::String::Format<64>("Foo: %d %c", foo.m_left, foo.m_right);
    }
}

bool UnitTests_UniquePtr_Class()
{
    auto deleter = [](Foo* pFooPtr)
        {
            //JPT_LOG("Deleted a Foo %s", pFooPtr->ToString().ConstBuffer());
            JPT_DELETE(pFooPtr);
        };

    auto deleter2 = [](Foo* pFooPtr)
        {
            //JPT_LOG("Another Deleted a Foo %s", pFooPtr->ToString().ConstBuffer());
            JPT_DELETE(pFooPtr);
        };

    jpt::UniquePtr<Foo> uniqueFoo = jpt::MakeUnique<Foo>(42, 'C');
    JPT_ENSURE(uniqueFoo.IsValid());
    JPT_ENSURE(uniqueFoo);
    JPT_ENSURE(uniqueFoo->m_left == 42);
    JPT_ENSURE(uniqueFoo->m_right == 'C');

    uniqueFoo.Reset(jpt::Allocator<Foo>::New(41, 'D'));
    JPT_ENSURE(uniqueFoo->m_left == 41);
    JPT_ENSURE(uniqueFoo->m_right == 'D');

    Foo* c = uniqueFoo.Release();
    JPT_ENSURE(!uniqueFoo);
    JPT_ENSURE(!uniqueFoo.IsValid());
    JPT_ENSURE(c->m_left == 41);
    JPT_ENSURE(c->m_right == 'D');
    JPT_DELETE(c);

    jpt::UniquePtr<Foo, decltype(deleter)> customDeleterUniquePtr = jpt::UniquePtr<Foo, decltype(deleter)>(jpt::Allocator<Foo>::New(42, 'C'), deleter);
    JPT_ENSURE(customDeleterUniquePtr.IsValid());
    JPT_ENSURE(customDeleterUniquePtr);
    JPT_ENSURE(customDeleterUniquePtr->m_left == 42);
    JPT_ENSURE(customDeleterUniquePtr->m_right == 'C');

    customDeleterUniquePtr.Reset(jpt::Allocator<Foo>::New(42, 'C'));
    JPT_ENSURE(customDeleterUniquePtr->m_left == 42);
    JPT_ENSURE(customDeleterUniquePtr->m_right == 'C');

    jpt::UniquePtr<Foo, decltype(deleter2)> otherCustomDeleterUniquePtr = jpt::UniquePtr<Foo, decltype(deleter2)>(jpt::Allocator<Foo>::New(42, 'C'), deleter2);

    otherCustomDeleterUniquePtr.GetDeleter()(customDeleterUniquePtr.Release());
    JPT_ENSURE(!customDeleterUniquePtr);
    JPT_ENSURE(!customDeleterUniquePtr.IsValid());
    JPT_ENSURE(otherCustomDeleterUniquePtr->m_left == 42);
    JPT_ENSURE(otherCustomDeleterUniquePtr->m_right == 'C');

    jpt::UniquePtr<Foo, decltype(deleter)> source = jpt::UniquePtr<Foo, decltype(deleter)>(jpt::Allocator<Foo>::New(42, 'a'), deleter);
    jpt::UniquePtr<Foo, decltype(deleter)> other = jpt::UniquePtr<Foo, decltype(deleter)>(jpt::Allocator<Foo>::New(43, 'c'), deleter);
    source = jpt::Move(other);

    jpt::UniquePtr<Foo[]> FooArray(jpt::Allocator<Foo>::NewArray(10, Foo{1, 'a'}, Foo{2, 'b'}, Foo(3, 'c'), Foo{4, 'd'}));
    JPT_ENSURE(FooArray.Get()[0] == Foo(1, 'a'));
    JPT_ENSURE(FooArray.Get()[1] == Foo(2, 'b'));
    JPT_ENSURE(FooArray.Get()[2] == Foo(3, 'c'));
    JPT_ENSURE(FooArray.Get()[3] == Foo(4, 'd'));
    JPT_ENSURE(FooArray.Get()[4] == Foo(0, '0'));
    JPT_ENSURE(FooArray.Get()[5] == Foo(0, '0'));

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