#include "JupiterPCH.h"
#include "MiscUnitTests.h"

bool UnitTest_String()
{
	jpt::string jupiterString("Jupiter Engine");
	JPT_RETURN_FALSE_IF_LOG(jupiterString.size() != 14, "");

	jpt::string anotherString("Zixuan Shi");
	JPT_RETURN_FALSE_IF_LOG(anotherString.size() != 10, "");

	jupiterString.append(" appended ");
	JPT_RETURN_FALSE_IF_LOG(jupiterString.size() != 24, "");

	jupiterString.append(anotherString);
	JPT_RETURN_FALSE_IF_LOG(jupiterString.size() != 34, "");

	JPT_RETURN_FALSE_IF_LOG(jupiterString.find_first_of("ter") != 4, "");
	JPT_RETURN_FALSE_IF_LOG(jupiterString.find_first_of("Zixuan") != 24, "");
	JPT_RETURN_FALSE_IF_LOG(jupiterString.find_first_of("does not exist") != jpt::string::npos, "");

	JPT_RETURN_FALSE_IF_LOG(jupiterString.substr(8, 6) != "Engine", "");

	jupiterString.clear();
	JPT_RETURN_FALSE_IF_LOG(jupiterString.size() != 0, "");
	JPT_RETURN_FALSE_IF_LOG(jupiterString.c_str() != nullptr, "");

	jupiterString.replace("a", "b");
	jupiterString = "Hello World oh my";
	jupiterString.replace("o", "Xo");
	JPT_RETURN_FALSE_IF_LOG(jupiterString != "HellXo WXorld Xoh my", "replace() failed");

	jupiterString.replace("Xo", "Xy", 6, 10);
	JPT_RETURN_FALSE_IF_LOG(jupiterString != "HellXo WXyrld Xoh my", "replace() failed");

	return true;
}

bool UnitTest_WString()
{
	jpt::wstring jupiterString(L"木星引擎");
	JPT_RETURN_FALSE_IF_LOG(jupiterString.size() != 4, "");

	jpt::wstring anotherString(L"石子渲");
	JPT_RETURN_FALSE_IF_LOG(anotherString.size() != 3, "");

	jupiterString.append(L" 附加 ");
	JPT_RETURN_FALSE_IF_LOG(jupiterString.size() != 8, "");

	jupiterString.append(anotherString);
	JPT_RETURN_FALSE_IF_LOG(jupiterString.size() != 11, "");

	JPT_RETURN_FALSE_IF_LOG(jupiterString.find_first_of(L"引") != 2, "");
	JPT_RETURN_FALSE_IF_LOG(jupiterString.find_first_of(L"子渲") != 9, "");
	JPT_RETURN_FALSE_IF_LOG(jupiterString.find_first_of(L"不存在") != jpt::wstring::npos, "");

	JPT_RETURN_FALSE_IF_LOG(jupiterString.substr(2, 2) != L"引擎", "");

	jupiterString.clear();
	JPT_RETURN_FALSE_IF_LOG(jupiterString.size() != 0, "");
	JPT_RETURN_FALSE_IF_LOG(jupiterString.c_str() != nullptr, "");

	return true;
}

bool UnitTest_StringView()
{
	jpt::string jptStr = "jpt string";

	jpt::basic_string_view jptStrView(jptStr.c_str() + 2, 3);
	JPT_RETURN_FALSE_IF_LOG(jptStrView != "t s", "");

	jpt::basic_string_view jptStrView2("jpt stringview raw");
	JPT_RETURN_FALSE_IF_LOG(jptStrView2 != "jpt stringview raw", "");

	jpt::basic_string_view jptStrView3(jptStr);
	JPT_RETURN_FALSE_IF_LOG(jptStrView3 != jptStr, "");

	return true;
}

bool UnitTest_Types()
{
	bool result = false;

	result = jpt::IsSameType<int32, float>::Value == false;
	JPT_RETURN_FALSE_IF_LOG(!result, "");

	result = jpt::IsSameType<int32, int32>::Value;
	JPT_RETURN_FALSE_IF_LOG(!result, "");

	result = jpt::IsSameType<jpt::string, const char*>::Value == false;
	JPT_RETURN_FALSE_IF_LOG(!result, "");

	result = jpt::IsSameType<jpt::string, jpt::string>::Value;
	JPT_RETURN_FALSE_IF_LOG(!result, "");

	return true;
}

bool UnitTest_Enum()
{
	JPT_ENUM(EFruit,
		Apple,
		Banana,
		Orange,
		Grape
		);

	EFruit fruit = EFruit::Apple;
	JPT_RETURN_FALSE_IF_LOG(fruit.ToString() != "Apple", "");
	JPT_RETURN_FALSE_IF_LOG(fruit.ToStringView() != "Apple", "");

	fruit = EFruit::Banana;
	JPT_RETURN_FALSE_IF_LOG(fruit.ToString() != "Banana", "");
	JPT_RETURN_FALSE_IF_LOG(fruit.ToStringView() != "Banana", "");

	fruit = EFruit::Orange;
	JPT_RETURN_FALSE_IF_LOG(fruit.ToString() != "Orange", "");
	JPT_RETURN_FALSE_IF_LOG(fruit.ToStringView() != "Orange", "");

	fruit = EFruit::Grape;
	JPT_RETURN_FALSE_IF_LOG(fruit.ToString() != "Grape", "");
	JPT_RETURN_FALSE_IF_LOG(fruit.ToStringView() != "Grape", "");

	for (EFruit i = EFruit::Start; i < EFruit::End; ++i)
	{
		switch (i)
		{
		case EFruit::Apple:
			JPT_RETURN_FALSE_IF_LOG(i.ToString() != "Apple", "");
			JPT_RETURN_FALSE_IF_LOG(i.ToStringView() != "Apple", "");
			break;

		case EFruit::Banana:
			JPT_RETURN_FALSE_IF_LOG(i.ToString() != "Banana", "");
			JPT_RETURN_FALSE_IF_LOG(i.ToStringView() != "Banana", "");
			break;

		case EFruit::Orange:
			JPT_RETURN_FALSE_IF_LOG(i.ToString() != "Orange", "");
			JPT_RETURN_FALSE_IF_LOG(i.ToStringView() != "Orange", "");
			break;

		case EFruit::Grape:
			JPT_RETURN_FALSE_IF_LOG(i.ToString() != "Grape", "");
			JPT_RETURN_FALSE_IF_LOG(i.ToStringView() != "Grape", "");
			break;
		}
	}

	//// Bitwise
	//JPT_ENUM(ECollisionType,
	//	NoCollision,
	//	Trigger         = 1 << 0,   
	//	Solid           = 1 << 1,
	//	Dynamic         = 1 << 2,
	//	Static          = 1 << 3,
	//	SimulatePhysics = 1 << 4,
	//	GenerateEvents  = 1 << 5
	//);

	//ECollisionType collision1 = ECollisionType::Trigger | ECollisionType::Static;
	//JPT_RETURN_FALSE_IF_LOG(!collision1.Has(ECollisionType::Trigger), "");
	//JPT_RETURN_FALSE_IF_LOG(!collision1.Has(ECollisionType::Static), "");
	//JPT_RETURN_FALSE_IF_LOG(collision1.Has(ECollisionType::Dynamic), "");

	//ECollisionType collision2 = ECollisionType::Dynamic;
	//ECollisionType collision3 = ECollisionType::SimulatePhysics | collision2;
	//JPT_RETURN_FALSE_IF_LOG(!collision2.Has(ECollisionType::Dynamic), "");
	//JPT_RETURN_FALSE_IF_LOG(!collision3.Has(ECollisionType::SimulatePhysics), "");
	//JPT_RETURN_FALSE_IF_LOG(!collision3.Has(collision2), "");


	return true;
}

bool UnitTest_Optional()
{
	jpt::optional<int32> intOptional;
	JPT_RETURN_FALSE_IF_LOG(intOptional.has_value(), "");

	intOptional = 42;
	JPT_RETURN_FALSE_IF_LOG(!intOptional.has_value(), "");
	JPT_RETURN_FALSE_IF_LOG(intOptional.value() != 42, "");

	intOptional.reset();
	JPT_RETURN_FALSE_IF_LOG(intOptional.has_value(), "");

	jpt::optional<jpt::string> stringOptional;
	JPT_RETURN_FALSE_IF_LOG(stringOptional.has_value(), "");

	stringOptional = "42";
	JPT_RETURN_FALSE_IF_LOG(!stringOptional.has_value(), "");
	JPT_RETURN_FALSE_IF_LOG(stringOptional.value() != "42", "");

	stringOptional.reset();
	JPT_RETURN_FALSE_IF_LOG(stringOptional.has_value(), "");

	return true;
}

void RunMiscUnitTests()
{
	JPT_RETURN_IF_LOG(!UnitTest_String(),     "UnitTest_String() failed");
	JPT_RETURN_IF_LOG(!UnitTest_WString(),    "UnitTest_WString() failed");
	JPT_RETURN_IF_LOG(!UnitTest_StringView(), "UnitTest_StringView() failed");
	JPT_RETURN_IF_LOG(!UnitTest_Types(),      "UnitTest_Types() failed");
	JPT_RETURN_IF_LOG(!UnitTest_Enum(),       "UnitTest_Enum() failed");
	JPT_RETURN_IF_LOG(!UnitTest_Optional(),   "UnitTest_Optional() failed");
}