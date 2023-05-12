#include "Core/Building/JupiterPCH.h"
#include "TypesUnitTests.h"

bool UnitTest_String()
{
	jpt::string jupiterString("Jupiter Engine");
	JPT_RUN_UNIT_TEST(jupiterString.size(), 14);

	jpt::string anotherString("Zixuan Shi");
	JPT_RUN_UNIT_TEST(anotherString.size(), 10);

	jupiterString.append(" appended ");
	JPT_RUN_UNIT_TEST(jupiterString.size(), 24);

	jupiterString.append(anotherString);
	JPT_RUN_UNIT_TEST(jupiterString.size(), 34);

	JPT_RUN_UNIT_TEST(jupiterString.find_first_of("ter"), 4);
	JPT_RUN_UNIT_TEST(jupiterString.find_last_of("Zixuan"), 24);
	JPT_RUN_UNIT_TEST(jupiterString.find("does not exist"), jpt::string::npos);

	JPT_RUN_UNIT_TEST(jupiterString.substr(8, 6), "Engine");

	jupiterString.clear();
	JPT_RUN_UNIT_TEST(jupiterString.size(), 0);
	JPT_RUN_UNIT_TEST(jupiterString.c_str(), nullptr);

	return true;
}

bool UnitTest_StringView()
{
	jpt::string jptStr = "jpt string";

	jpt::string_view jptStrView(jptStr.c_str() + 2, 3);
	JPT_CHECK_RESULT(jptStrView == "t s");

	jpt::string_view jptStrView2("jpt stringview raw");
	JPT_CHECK_RESULT(jptStrView2 == "jpt stringview raw");

	jpt::string_view jptStrView3(jptStr);
	JPT_CHECK_RESULT(jptStrView3 == jptStr);

	return true;
}

void RunTypesUnitTests()
{
	JPT_RUN_UNIT_TESTS(UnitTest_String);
	JPT_RUN_UNIT_TESTS(UnitTest_StringView);
}