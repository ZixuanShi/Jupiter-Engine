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

bool UnitTest_StringView()
{
	jpt::string jptStr = "jpt string";

	jpt::string_view jptStrView(jptStr.c_str() + 2, 3);
	JPT_RETURN_FALSE_IF_LOG(jptStrView != "t s", "");

	jpt::string_view jptStrView2("jpt stringview raw");
	JPT_RETURN_FALSE_IF_LOG(jptStrView2 != "jpt stringview raw", "");

	jpt::string_view jptStrView3(jptStr);
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
	return true;
}

void RunTypesUnitTests()
{
	JPT_RETURN_IF_LOG(!UnitTest_String(), "");
	JPT_RETURN_IF_LOG(!UnitTest_StringView(), "");
	JPT_RETURN_IF_LOG(!UnitTest_Types(), "");
	JPT_RETURN_IF_LOG(!UnitTest_Enum(), "");
}