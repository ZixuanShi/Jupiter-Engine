#include "Core/Building/JupiterPCH.h"
#include "AlgorithmsUnitTests.h"

bool UnitTest_strlen()
{
	JPT_RUN_UNIT_TEST(jpt::strlen(""), 0);
	JPT_RUN_UNIT_TEST(jpt::strlen("0"), 1);
	JPT_RUN_UNIT_TEST(jpt::strlen("12345"), 5);
	return true;
}

bool UnitTest_itoa()
{
	char* itoaResult = jpt::itoa(12345);
	JPT_CHECK_RESULT(strcmp(itoaResult, "12345") == 0);
	delete[] itoaResult;

	itoaResult = jpt::itoa(-12345);
	JPT_CHECK_RESULT(strcmp(itoaResult, "-12345") == 0);
	delete[] itoaResult;

	itoaResult = jpt::itoa(0);
	JPT_CHECK_RESULT(strcmp(itoaResult, "0") == 0);
	delete[] itoaResult;

	return true;
}

bool UnitTest_to_string()
{
	JPT_RUN_UNIT_TEST(jpt::to_string(0), jpt::string("0"));
	JPT_RUN_UNIT_TEST(jpt::to_string(0), "0");
	JPT_RUN_UNIT_TEST(jpt::to_string(12345), jpt::string("12345"));
	JPT_RUN_UNIT_TEST(jpt::to_string(12345), "12345");
	JPT_RUN_UNIT_TEST(jpt::to_string(-12345), jpt::string("-12345"));
	JPT_RUN_UNIT_TEST(jpt::to_string(-12345), "-12345");
	return true;
}

void RunAlgorithmsUnitTests()
{
	JPT_RUN_UNIT_TESTS(UnitTest_strlen);
	JPT_RUN_UNIT_TESTS(UnitTest_itoa);
	JPT_RUN_UNIT_TESTS(UnitTest_to_string);
}
