// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_TypeTraits;

import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Utilities;
import jpt.Vector2;
import jpt.Vector3;
import jpt.Function;
import jpt.Slot;
import jpt.String;
import jpt.StringView;
import jpt.StaticArray;

template<typename T>
bool UnitTest_TRemoveTraits()
{
	bool value = false;

	value = jpt::AreSameType<jpt::TRValueToLValueReference<T>, T>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<jpt::TRValueToLValueReference<T&&>, T&>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<decltype(jpt::TRemoveReference<T>), T>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<decltype(jpt::TRemoveReference<T&>), T>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<decltype(jpt::TRemoveReference<T&&>), T>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<decltype(jpt::TRemoveReference<const T&>), const T>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<decltype(jpt::TRemoveConst<const T>), T>;
	JPT_ENSURE(value);

	// https://stackoverflow.com/questions/15887144/stdremove-const-with-const-references
	{
		value = !jpt::AreSameType<decltype(jpt::TRemoveConst<const T&>), T&>;
		JPT_ENSURE(value);

		T t = T();
		const T& constRefT = t;
		jpt::TRemoveReference<decltype(constRefT)> constT = t;

		value = jpt::AreSameType<decltype(constT), const T>;
		JPT_ENSURE(value);

		jpt::TRemoveConst<decltype(constT)> plainT = t;
		value = jpt::AreSameType<decltype(plainT), T>;
		JPT_ENSURE(value);
	}

	return true;
}

template<typename T>
bool UnitTest_TDecay()
{
	bool value = false;

	value = jpt::AreSameType<jpt::TDecay<T>, T>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<jpt::TDecay<T&>, T>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<jpt::TDecay<T&&>, T>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<jpt::TDecay<const T>, T>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<jpt::TDecay<const T&>, T>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<jpt::TDecay<const T&&>, T>;
	JPT_ENSURE(value);

	return true;
}

bool UnitTest_IsLValueRefType()
{
	bool value = false;

	value = jpt::IsLValueRef<int32>;
	JPT_ENSURE(!value);

	value = jpt::IsLValueRef<int32&>;
	JPT_ENSURE(value);

	return true;
}

bool UnitTest_IsRValueRefType()
{
	bool value = false;

	value = jpt::IsRValueRef<int32>;
	JPT_ENSURE(value);

	value = jpt::IsRValueRef<int32&&>;
	JPT_ENSURE(value);

	return true;
}

bool UnitTest_IsRef()
{
	bool value = false;

	value = jpt::IsRef<int32>;
	JPT_ENSURE(!value);

	value = jpt::IsRef<int32&>;
	JPT_ENSURE(value);

	value = jpt::IsRef<int32&&>;
	JPT_ENSURE(value);

	return true;
}

bool UnitTest_AreSameType()
{
	bool value = false;

	value = jpt::AreSameType<int32, int32>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<int32, int32&>;
	JPT_ENSURE(!value);

	value = jpt::AreSameType<int32&, int32&>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<int32, float>;
	JPT_ENSURE(!value);

	value = jpt::AreSameType<float, float>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<jpt::String, jpt::String>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<jpt::String&, jpt::String&>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<jpt::String&&, jpt::String&&>;
	JPT_ENSURE(value);

	value = jpt::AreSameType<jpt::String&, jpt::String&&>;
	JPT_ENSURE(!value);

	value = jpt::AreSameType<jpt::String, jpt::String&&>;
	JPT_ENSURE(!value);

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
			return jpt::IsAnyOf<jpt::TDecay<decltype(var)>, int32, float, char, jpt::String>;
		};

	Foo<int32, float, char> foo;
	JPT_ENSURE(foo.Work<int32>());
	JPT_ENSURE(!foo.Work<jpt::String>());

	int32 num = 10;
	value = helper(num);
	JPT_ENSURE(value);
	value = helper(10);
	JPT_ENSURE(value);

	float f = 10.0f;
	value = helper(f);
	JPT_ENSURE(value);
	value = helper(10.0f);
	JPT_ENSURE(value);

	double d = 10.0;
	value = helper(d);
	JPT_ENSURE(!value);
	value = helper(10.0);
	JPT_ENSURE(!value);

	const char* cstr = "Hello";
	value = helper(cstr);
	JPT_ENSURE(!value);
	value = helper("Hello");
	JPT_ENSURE(!value);

	jpt::String str = "Hello";
	value = helper(str);
	JPT_ENSURE(value);
	value = helper(jpt::String("Hello"));
	JPT_ENSURE(value);

	jpt::WString wstr = L"Hello";
	value = helper(wstr);
	JPT_ENSURE(!value);
	value = helper(jpt::WString(L"Hello"));
	JPT_ENSURE(!value);

	bool b = false;
	value = helper(b);
	JPT_ENSURE(!value);
	value = helper(false);
	JPT_ENSURE(!value);

	return true;
}

bool UnitTest_IsArray()
{
	int32 num = 0;
	JPT_ENSURE(!jpt::IsArray<decltype(num)>);

	int32 numArray[] = { 0,1 };
	JPT_ENSURE(jpt::IsArray<decltype(numArray)>);
	
	int32 numArray2[2] = { 0,1 };
	JPT_ENSURE(jpt::IsArray<decltype(numArray2)>);

	int32* num2 = new int;
	JPT_ENSURE(!jpt::IsArray<decltype(num2)>);
	JPT_DELETE(num2);

	int32* numArray3 = new int[2];
	JPT_ENSURE(!jpt::IsArray<decltype(numArray3)>);
	JPT_DELETE(numArray3);

	return true;
}

struct SimpleStruct1
{
	int32 a;
	float32 b;
};
struct SimpleStruct2
{
	SimpleStruct2(int32 n) { JPT_IGNORE(n); };
};
template<>
constexpr bool jpt::IsTrivial<SimpleStruct2> = true;

struct ComplexStruct1
{
	int32 a;
	jpt::String str;
};
struct ComplexStruct2
{
	ComplexStruct2(int32 n) { JPT_IGNORE(n); };
};

bool UnitTest_IsTrivial()
{
	JPT_ENSURE(jpt::IsTrivial<int32>);
	JPT_ENSURE(jpt::IsTrivial<int64>);
	JPT_ENSURE(jpt::IsTrivial<uint32>);
	JPT_ENSURE(jpt::IsTrivial<uint64>);
	JPT_ENSURE(jpt::IsTrivial<float32>);
	JPT_ENSURE(jpt::IsTrivial<float64>);
	JPT_ENSURE(jpt::IsTrivial<Vec2f>);
	JPT_ENSURE(jpt::IsTrivial<Vec3f>);
	JPT_ENSURE(jpt::IsTrivial<SimpleStruct1>);
	JPT_ENSURE(jpt::IsTrivial<SimpleStruct2>);

	JPT_ENSURE(!jpt::IsTrivial<jpt::String>);
	JPT_ENSURE(!jpt::IsTrivial<jpt::WString>);
	JPT_ENSURE(!jpt::IsTrivial<jpt::StringView>);
	JPT_ENSURE(!jpt::IsTrivial<jpt::WStringView>);
	JPT_ENSURE(!jpt::IsTrivial<jpt::DynamicArray<int32>>);
	JPT_ENSURE(!jpt::IsTrivial<jpt::DynamicArray<jpt::String>>);
	JPT_ENSURE(!(jpt::IsTrivial<jpt::StaticArray<jpt::String, 5>>));
	JPT_ENSURE(!(jpt::IsTrivial<jpt::StaticArray<int32, 5>>));
	JPT_ENSURE(!(jpt::IsTrivial<jpt::HashMap<jpt::String, int32>>));
	JPT_ENSURE(!jpt::IsTrivial<jpt::Function<void()>>);
	JPT_ENSURE(!jpt::IsTrivial<jpt::Slot<void()>>);
	JPT_ENSURE(!jpt::IsTrivial<ComplexStruct1>);
	JPT_ENSURE(!jpt::IsTrivial<ComplexStruct2>);

	return true;
}

export bool RunUnitTests_TypeTraits()
{
	JPT_ENSURE(UnitTest_TRemoveTraits<int32>());
	JPT_ENSURE(UnitTest_TRemoveTraits<jpt::String>());

	JPT_ENSURE(UnitTest_TDecay<int32>());
	JPT_ENSURE(UnitTest_TDecay<jpt::String>());

	JPT_ENSURE(UnitTest_IsLValueRefType());
	JPT_ENSURE(UnitTest_IsLValueRefType());
	JPT_ENSURE(UnitTest_IsRef());
	JPT_ENSURE(UnitTest_AreSameType());
	JPT_ENSURE(UnitTest_IsAnyOf());
	JPT_ENSURE(UnitTest_IsArray());
	JPT_ENSURE(UnitTest_IsTrivial());

	return true;
}