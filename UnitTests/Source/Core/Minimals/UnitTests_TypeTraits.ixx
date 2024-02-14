// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_TypeTraits;

import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.String;

template<typename T>
bool UnitTest_RemoveTraits()
{
	bool value = false;

	value = jpt::IsSameType<typename jpt::RemoveReference<T>::Type, T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<typename jpt::RemoveReference<T&>::Type, T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<typename jpt::RemoveReference<T&&>::Type, T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<typename jpt::RemoveConst<T>::Type, T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<typename jpt::RemoveConst<const T>::Type, T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<typename jpt::RTValueoLValueReference<T>::Type, T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<typename jpt::RTValueoLValueReference<T&&>::Type, T&>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<typename jpt::RemoveReference<const T&>::Type, const T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	typename jpt::RemoveConst<const T&>::Type foo = T();
	value = jpt::IsSameType<decltype(foo), const T&>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	typename jpt::RemoveReference<decltype(foo)>::Type bar = foo;
	value = jpt::IsSameType<decltype(bar), const T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	typename jpt::RemoveConst<decltype(bar)>::Type baz = foo;
	value = jpt::IsSameType<decltype(baz), T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	return true;
}

template<typename T>
bool UnitTest_Decay()
{
	bool value = false;

	value = jpt::IsSameType<typename jpt::Decay<T>::Type, T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<typename jpt::Decay<T&>::Type, T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<typename jpt::Decay<T&&>::Type, T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<typename jpt::Decay<const T>::Type, T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<typename jpt::Decay<const T&>::Type, T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<typename jpt::Decay<const T&&>::Type, T>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	return true;
}

bool UnitTest_IsLValueRefType()
{
	bool value = false;

	value = jpt::IsLValueRef<int32>;
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	value = jpt::IsLValueRef<int32&>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	return true;
}

bool UnitTest_IsRValueRefType()
{
	bool value = false;

	value = jpt::IsRValueRef<int32>;
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	value = jpt::IsRValueRef<int32&&>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	return true;
}

bool UnitTest_IsRef()
{
	bool value = false;

	value = jpt::IsRef<int32>;
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	value = jpt::IsRef<int32&>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsRef<int32&&>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	return true;
}

bool UnitTest_IsSameType()
{
	bool value = false;

	value = jpt::IsSameType<int32, int32>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<int32, int32&>;
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	value = jpt::IsSameType<int32&, int32&>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<int32, float>;
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	value = jpt::IsSameType<float, float>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<jpt::String, jpt::String>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<jpt::String&, jpt::String&>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<jpt::String&&, jpt::String&&>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::IsSameType<jpt::String&, jpt::String&&>;
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	value = jpt::IsSameType<jpt::String, jpt::String&&>;
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	return true;
}

bool UnitTest_NotSameType()
{
	bool value = false;

	value = jpt::NotSameType<int32, int32>;
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	value = jpt::NotSameType<int32, int32&>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::NotSameType<int32&, int32&>;
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	value = jpt::NotSameType<int32, float>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::NotSameType<float, float>;
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	value = jpt::NotSameType<jpt::String, jpt::String>;
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	value = jpt::NotSameType<jpt::String&, jpt::String&>;
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	value = jpt::NotSameType<jpt::String&&, jpt::String&&>;
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	value = jpt::NotSameType<jpt::String&, jpt::String&&>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	value = jpt::NotSameType<jpt::String, jpt::String&&>;
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	return true;
}

template<typename ...TArgs>
struct Foo
{
	template<typename T>
	bool Work()
	{
		return jpt::IsAnyOf<T, TArgs...>;
	}
};

bool UnitTest_IsAnyOf()
{
	bool value = false;
	auto helper = [](const auto& var) -> bool
		{
			return jpt::IsAnyOf<typename jpt::Decay<decltype(var)>::Type, int32, float, char, jpt::String>;
		};

	Foo<int32, float, char> foo;
	JPT_RETURN_FALSE_IF_ERROR(!foo.Work<int32>(), "");
	JPT_RETURN_FALSE_IF_ERROR(foo.Work<jpt::String>(), "");

	int32 num = 10;
	value = helper(num);
	JPT_RETURN_FALSE_IF_ERROR(!value, "");
	value = helper(10);
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	float f = 10.0f;
	value = helper(f);
	JPT_RETURN_FALSE_IF_ERROR(!value, "");
	value = helper(10.0f);
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	double d = 10.0;
	value = helper(d);
	JPT_RETURN_FALSE_IF_ERROR(value, "");
	value = helper(10.0);
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	const char* cstr = "Hello";
	value = helper(cstr);
	JPT_RETURN_FALSE_IF_ERROR(value, "");
	value = helper("Hello");
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	jpt::String str = "Hello";
	value = helper(str);
	JPT_RETURN_FALSE_IF_ERROR(!value, "");
	value = helper(jpt::String("Hello"));
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	jpt::WString wstr = L"Hello";
	value = helper(wstr);
	JPT_RETURN_FALSE_IF_ERROR(value, "");
	value = helper(jpt::WString(L"Hello"));
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	bool b = false;
	value = helper(b);
	JPT_RETURN_FALSE_IF_ERROR(value, "");
	value = helper(false);
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	return true;
}

bool UnitTest_NotAnyOf()
{
	bool value = false;
	auto helper = [](const auto& var) -> bool
		{
			return jpt::NotAnyOf<typename jpt::Decay<decltype(var)>::Type, int32, float, char, jpt::String>;
		};

	int32 num = 10;
	value = helper(num);
	JPT_RETURN_FALSE_IF_ERROR(value, "");
	value = helper(10);
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	float f = 10.0f;
	value = helper(f);
	JPT_RETURN_FALSE_IF_ERROR(value, "");
	value = helper(10.0f);
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	double d = 10.0;
	value = helper(d);
	JPT_RETURN_FALSE_IF_ERROR(!value, "");
	value = helper(10.0);
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	const char* cstr = "Hello";
	value = helper(cstr);
	JPT_RETURN_FALSE_IF_ERROR(!value, "");
	value = helper("Hello");
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	jpt::String str = "Hello";
	value = helper(str);
	JPT_RETURN_FALSE_IF_ERROR(value, "");
	value = helper(jpt::String("Hello"));
	JPT_RETURN_FALSE_IF_ERROR(value, "");

	jpt::WString wstr = L"Hello";
	value = helper(wstr);
	JPT_RETURN_FALSE_IF_ERROR(!value, "");
	value = helper(jpt::WString(L"Hello"));
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	bool b = false;
	value = helper(b);
	JPT_RETURN_FALSE_IF_ERROR(!value, "");
	value = helper(false);
	JPT_RETURN_FALSE_IF_ERROR(!value, "");

	return true;
}

bool UnitTest_IsEmptyObj()
{
	class Foo
	{
	};

	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsEmptyObj<Foo>, "");

	class Bar
	{
		int32 m_data = 10;
	};
	JPT_RETURN_FALSE_IF_ERROR(jpt::IsEmptyObj<Bar>, "");

	class Baz
	{
	public:
		Baz() = default;
		Baz(int32 data) {JPT_IGNORE(data); }
	};
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsEmptyObj<Baz>, "");

	class A
	{
	public:
		virtual ~A() = default;
	};
	JPT_RETURN_FALSE_IF_ERROR(jpt::IsEmptyObj<A>, "");

	class B : public A
	{
	};
	JPT_RETURN_FALSE_IF_ERROR(jpt::IsEmptyObj<B>, "");

	class C
	{
	public:
		void Work() {}
	};
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsEmptyObj<C>, "");

	class D
	{
	public:
		static void Work() {}
	};
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsEmptyObj<D>, "");

	return true;
}

bool UnitTest_IsArray()
{
	int32 num = 0;
	JPT_RETURN_FALSE_IF_ERROR(jpt::IsArray<decltype(num)>, "");

	int32 numArray[] = { 0,1 };
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsArray<decltype(numArray)>, "");
	
	int32 numArray2[2] = { 0,1 };
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsArray<decltype(numArray2)>, "");

	int32* num2 = new int;
	JPT_RETURN_FALSE_IF_ERROR(jpt::IsArray<decltype(num2)>, "");
	JPT_DELETE(num2);

	int32* numArray3 = new int[2];
	JPT_RETURN_FALSE_IF_ERROR(jpt::IsArray<decltype(numArray3)>, "");
	JPT_DELETE(numArray3);

	return true;
}

export bool RunUnitTests_TypeTraits()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_RemoveTraits<int32>(), "UnitTest_RemoveTraits Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_RemoveTraits<jpt::String>(), "UnitTest_RemoveTraits Failed");

	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Decay<int32>(), "UnitTest_Decay Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Decay<jpt::String>(), "UnitTest_Decay Failed");

	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_IsLValueRefType(), "UnitTest_IsLValueRefType Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_IsLValueRefType(), "UnitTest_IsLValueRefType Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_IsRef(), "UnitTest_IsRef Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_IsSameType(), "UnitTest_IsSameType Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_NotSameType(), "UnitTest_NotSameType Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_IsAnyOf(), "UnitTest_IsAnyOf Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_NotAnyOf(), "UnitTest_NotAnyOf Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_IsEmptyObj(), "UnitTest_IsEmptyObj Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_IsArray(), "UnitTest_IsArray Failed");

	return true;
}