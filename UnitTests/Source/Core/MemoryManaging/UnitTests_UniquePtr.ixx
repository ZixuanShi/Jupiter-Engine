// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_UniquePtr;

import jpt.Allocator;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.UniquePtr;
import jpt.ToString;
import jpt.String;

bool UnitTest_UniquePtr_Char()
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
	JPT_RETURN_FALSE_IF_ERROR(!uniqueChar.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!uniqueChar, "");
	JPT_RETURN_FALSE_IF_ERROR(*uniqueChar != 'C', "");

	uniqueChar.Reset(new char('D'));
	JPT_RETURN_FALSE_IF_ERROR(*uniqueChar != 'D', "");

	char* c = uniqueChar.Release();
	JPT_RETURN_FALSE_IF_ERROR(uniqueChar, "");
	JPT_RETURN_FALSE_IF_ERROR(uniqueChar.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(*c != 'D', "");
	delete c;

	jpt::UniquePtr<char, decltype(deleter)> customDeleterUniquePtr = jpt::UniquePtr<char, decltype(deleter)>(new char('C'), deleter);
	JPT_RETURN_FALSE_IF_ERROR(!customDeleterUniquePtr.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!customDeleterUniquePtr, "");
	JPT_RETURN_FALSE_IF_ERROR(*customDeleterUniquePtr != 'C', "");

	customDeleterUniquePtr.Reset(new char('D'));
	JPT_RETURN_FALSE_IF_ERROR(*customDeleterUniquePtr != 'D', "");

	jpt::UniquePtr<char, decltype(deleter2)> otherCustomDeleterUniquePtr = jpt::UniquePtr<char, decltype(deleter2)>(new char('D'), deleter2);

	otherCustomDeleterUniquePtr.GetDeleter()(customDeleterUniquePtr.Release());
	JPT_RETURN_FALSE_IF_ERROR(customDeleterUniquePtr, "");
	JPT_RETURN_FALSE_IF_ERROR(customDeleterUniquePtr.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(*otherCustomDeleterUniquePtr != 'D', "");

	jpt::UniquePtr<char, decltype(deleter)> source = jpt::UniquePtr<char, decltype(deleter)>(new char('A'), deleter);
	jpt::UniquePtr<char, decltype(deleter)> other  = jpt::UniquePtr<char, decltype(deleter)>(new char('B'), deleter);
	source = jpt::Move(other);
	JPT_RETURN_FALSE_IF_ERROR(*source != 'B', "");

	jpt::UniquePtr<char[]> charArray(jpt::Allocator<char>::AllocateArray(10, {'A', 'B', 'C', 'A'}));
	//JPT_LOG(charArray.Get()[0]);
	//JPT_LOG(charArray.Get()[1]);
	//JPT_LOG(charArray.Get()[2]);
	//JPT_LOG(charArray.Get()[3]);
	//JPT_LOG(charArray.Get()[4]);
	//JPT_LOG(charArray.Get()[5]);

	return true;
}

bool UnitTest_UniquePtr_Int()
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
	JPT_RETURN_FALSE_IF_ERROR(!uniqueint32.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!uniqueint32, "");
	JPT_RETURN_FALSE_IF_ERROR(*uniqueint32 != 42, "");

	uniqueint32.Reset(new int32(42));
	JPT_RETURN_FALSE_IF_ERROR(*uniqueint32 != 42, "");

	int32* c = uniqueint32.Release();
	JPT_RETURN_FALSE_IF_ERROR(uniqueint32, "");
	JPT_RETURN_FALSE_IF_ERROR(uniqueint32.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(*c != 42, "");
	delete c;

	jpt::UniquePtr<int32, decltype(deleter)> customDeleterUniquePtr = jpt::UniquePtr<int32, decltype(deleter)>(new int32(42), deleter);
	JPT_RETURN_FALSE_IF_ERROR(!customDeleterUniquePtr.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!customDeleterUniquePtr, "");
	JPT_RETURN_FALSE_IF_ERROR(*customDeleterUniquePtr != 42, "");

	customDeleterUniquePtr.Reset(new int32(42));
	JPT_RETURN_FALSE_IF_ERROR(*customDeleterUniquePtr != 42, "");

	jpt::UniquePtr<int32, decltype(deleter2)> otherCustomDeleterUniquePtr = jpt::UniquePtr<int32, decltype(deleter2)>(new int32(42), deleter2);

	otherCustomDeleterUniquePtr.GetDeleter()(customDeleterUniquePtr.Release());
	JPT_RETURN_FALSE_IF_ERROR(customDeleterUniquePtr, "");
	JPT_RETURN_FALSE_IF_ERROR(customDeleterUniquePtr.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(*otherCustomDeleterUniquePtr != 42, "");

	jpt::UniquePtr<int32, decltype(deleter)> source = jpt::UniquePtr<int32, decltype(deleter)>(new int32(105), deleter);
	jpt::UniquePtr<int32, decltype(deleter)> other = jpt::UniquePtr<int32, decltype(deleter)>(new int32(101), deleter);
	source = jpt::Move(other);

	jpt::UniquePtr<int32[]> int32Array(jpt::Allocator<int32>::AllocateArray(10, { 1, 2, 3, 4 }));
	//JPT_LOG(int32Array.Get()[0]);
	//JPT_LOG(int32Array.Get()[1]);
	//JPT_LOG(int32Array.Get()[2]);
	//JPT_LOG(int32Array.Get()[3]);
	//JPT_LOG(int32Array.Get()[4]);
	//JPT_LOG(int32Array.Get()[5]);

	return true;
}

bool UnitTest_UniquePtr_String()
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
	JPT_RETURN_FALSE_IF_ERROR(!uniqueString.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!uniqueString, "");
	JPT_RETURN_FALSE_IF_ERROR(*uniqueString != "Jupiter", "");

	uniqueString.Reset(new String("Engine"));
	JPT_RETURN_FALSE_IF_ERROR(*uniqueString != "Engine", "");

	String* c = uniqueString.Release();
	JPT_RETURN_FALSE_IF_ERROR(uniqueString, "");
	JPT_RETURN_FALSE_IF_ERROR(uniqueString.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(*c != "Engine", "");
	delete c;

	jpt::UniquePtr<String, decltype(deleter)> customDeleterUniquePtr = jpt::UniquePtr<String, decltype(deleter)>(new String("Jupiter"), deleter);
	JPT_RETURN_FALSE_IF_ERROR(!customDeleterUniquePtr.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!customDeleterUniquePtr, "");
	JPT_RETURN_FALSE_IF_ERROR(*customDeleterUniquePtr != "Jupiter", "");

	customDeleterUniquePtr.Reset(new String("Engine"));
	JPT_RETURN_FALSE_IF_ERROR(*customDeleterUniquePtr != "Engine", "");

	jpt::UniquePtr<String, decltype(deleter2)> otherCustomDeleterUniquePtr = jpt::UniquePtr<String, decltype(deleter2)>(new String("Engine"), deleter2);

	otherCustomDeleterUniquePtr.GetDeleter()(customDeleterUniquePtr.Release());
	JPT_RETURN_FALSE_IF_ERROR(customDeleterUniquePtr, "");
	JPT_RETURN_FALSE_IF_ERROR(customDeleterUniquePtr.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(*otherCustomDeleterUniquePtr != "Engine", "");

	jpt::UniquePtr<String, decltype(deleter)> source = jpt::UniquePtr<String, decltype(deleter)>(new String("105"), deleter);
	jpt::UniquePtr<String, decltype(deleter)> other = jpt::UniquePtr<String, decltype(deleter)>(new String("101"), deleter);
	source = jpt::Move(other);

	return true;
}

bool UnitTest_UniquePtr_Class()
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
	JPT_RETURN_FALSE_IF_ERROR(!uniqueFoo.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!uniqueFoo, "");
	JPT_RETURN_FALSE_IF_ERROR(uniqueFoo->m_left != 42, "");
	JPT_RETURN_FALSE_IF_ERROR(uniqueFoo->m_right != 'C', "");

	uniqueFoo.Reset(new Foo(41, 'D'));
	JPT_RETURN_FALSE_IF_ERROR(uniqueFoo->m_left != 41, "");
	JPT_RETURN_FALSE_IF_ERROR(uniqueFoo->m_right != 'D', "");

	Foo* c = uniqueFoo.Release();
	JPT_RETURN_FALSE_IF_ERROR(uniqueFoo, "");
	JPT_RETURN_FALSE_IF_ERROR(uniqueFoo.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(c->m_left != 41, "");
	JPT_RETURN_FALSE_IF_ERROR(c->m_right != 'D', "");
	delete c;

	jpt::UniquePtr<Foo, decltype(deleter)> customDeleterUniquePtr = jpt::UniquePtr<Foo, decltype(deleter)>(new Foo(42, 'C'), deleter);
	JPT_RETURN_FALSE_IF_ERROR(!customDeleterUniquePtr.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(!customDeleterUniquePtr, "");
	JPT_RETURN_FALSE_IF_ERROR(customDeleterUniquePtr->m_left != 42, "");
	JPT_RETURN_FALSE_IF_ERROR(customDeleterUniquePtr->m_right != 'C', "");

	customDeleterUniquePtr.Reset(new Foo(42, 'C'));
	JPT_RETURN_FALSE_IF_ERROR(customDeleterUniquePtr->m_left != 42, "");
	JPT_RETURN_FALSE_IF_ERROR(customDeleterUniquePtr->m_right != 'C', "");

	jpt::UniquePtr<Foo, decltype(deleter2)> otherCustomDeleterUniquePtr = jpt::UniquePtr<Foo, decltype(deleter2)>(new Foo(42, 'C'), deleter2);

	otherCustomDeleterUniquePtr.GetDeleter()(customDeleterUniquePtr.Release());
	JPT_RETURN_FALSE_IF_ERROR(customDeleterUniquePtr, "");
	JPT_RETURN_FALSE_IF_ERROR(customDeleterUniquePtr.IsValid(), "");
	JPT_RETURN_FALSE_IF_ERROR(otherCustomDeleterUniquePtr->m_left != 42, "");
	JPT_RETURN_FALSE_IF_ERROR(otherCustomDeleterUniquePtr->m_right != 'C', "");

	jpt::UniquePtr<Foo, decltype(deleter)> source = jpt::UniquePtr<Foo, decltype(deleter)>(new Foo(42, 'a'), deleter);
	jpt::UniquePtr<Foo, decltype(deleter)> other = jpt::UniquePtr<Foo, decltype(deleter)>(new Foo(43, 'c'), deleter);
	source = jpt::Move(other);

	jpt::UniquePtr<Foo[]> FooArray(jpt::Allocator<Foo>::AllocateArray(10, { {1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'} }));
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
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_UniquePtr_Char(), "UnitTest_UniquePtr_Char Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_UniquePtr_Int(), "UnitTest_UniquePtr_Int Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_UniquePtr_String(), "UnitTest_UniquePtr_String Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_UniquePtr_Class(), "UnitTest_UniquePtr_Class Failed");

	return true;
}