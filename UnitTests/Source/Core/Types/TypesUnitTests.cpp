#include "Core/Building/JupiterPCH.h"
#include "TypesUnitTests.h"

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

	fruit = EFruit::Banana;
	JPT_RETURN_FALSE_IF_LOG(fruit.ToString() != "Banana", "");

	fruit = EFruit::Orange;
	JPT_RETURN_FALSE_IF_LOG(fruit.ToString() != "Orange", "");

	fruit = EFruit::Grape;
	JPT_RETURN_FALSE_IF_LOG(fruit.ToString() != "Grape", "");

	for (EFruit i = EFruit::Start; i < EFruit::End; ++i)
	{
		switch (i)
		{
		case EFruit::Apple:
			JPT_RETURN_FALSE_IF_LOG(i.ToString() != "Apple", "");
			break;

		case EFruit::Banana:
			JPT_RETURN_FALSE_IF_LOG(i.ToString() != "Banana", "");
			break;

		case EFruit::Orange:
			JPT_RETURN_FALSE_IF_LOG(i.ToString() != "Orange", "");
			break;

		case EFruit::Grape:
			JPT_RETURN_FALSE_IF_LOG(i.ToString() != "Grape", "");
			break;
		}
	}

	return true;
}

void RunTypesUnitTests()
{
	JPT_RETURN_IF_LOG(!UnitTest_String(), "UnitTest_String() failed");
	JPT_RETURN_IF_LOG(!UnitTest_WString(), "UnitTest_WString() failed");
	JPT_RETURN_IF_LOG(!UnitTest_StringView(), "UnitTest_StringView() failed");
	JPT_RETURN_IF_LOG(!UnitTest_Types(), "UnitTest_Types() failed");
	JPT_RETURN_IF_LOG(!UnitTest_Enum(), "UnitTest_Enum() failed");
}