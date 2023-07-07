#include "JupiterPCH.h"
#include "AlgorithmsUnitTests.h"

bool UnitTest_strlen()
{
	JPT_RETURN_FALSE_IF_LOG(jpt::strlen("") != 0, "");
	JPT_RETURN_FALSE_IF_LOG(jpt::strlen("0") != 1, "");
	JPT_RETURN_FALSE_IF_LOG(jpt::strlen("12345") != 5, "");
	return true;
}

bool UnitTest_wcslen()
{
	JPT_RETURN_FALSE_IF_LOG(jpt::wcslen(L"") != 0, "");
	JPT_RETURN_FALSE_IF_LOG(jpt::wcslen(L"0") != 1, "");
	JPT_RETURN_FALSE_IF_LOG(jpt::wcslen(L"12345") != 5, "");
	return true;
}

bool UnitTest_itoa()
{
	char* itoaResult = jpt::IntegerToCStr(12345);
	JPT_RETURN_FALSE_IF_LOG(strcmp(itoaResult, "12345") != 0, "");
	delete[] itoaResult;

	itoaResult = jpt::IntegerToCStr(-12345);
	JPT_RETURN_FALSE_IF_LOG(strcmp(itoaResult, "-12345") != 0, "");
	delete[] itoaResult;

	itoaResult = jpt::IntegerToCStr(0);
	JPT_RETURN_FALSE_IF_LOG(strcmp(itoaResult, "0") != 0, "");
	delete[] itoaResult;

	return true;
}

bool UnitTest_to_string()
{
	// Integer
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(0) != jpt::string("0"), "");
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(0) != "0", "");
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(12345) != jpt::string("12345"), "");
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(12345) != "12345", "");
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(-12345) != "-12345", "");
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(-12345) != "-12345", "");

	// Float
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(0.0f) != "0.000", "");
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(-0.0f) != "-0.000", "");
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(0.01f) != "0.010", "");
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(0.012) != "0.012", "");
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(0.01232522352f) != "0.012", "");
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(-0.01f) != "-0.010", "");
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(-0.012) != "-0.012", "");
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(-0.01232522352) != "-0.012", "");
	JPT_RETURN_FALSE_IF_LOG(jpt::ToString(114514.0) != "114514.000", "");

	return true;
}

void RunAlgorithmsUnitTests()
{
	JPT_RETURN_IF_LOG(!UnitTest_strlen(), "UnitTest_strlen failed");
	JPT_RETURN_IF_LOG(!UnitTest_wcslen(), "UnitTest_wcslen failed");
	JPT_RETURN_IF_LOG(!UnitTest_itoa(), "UnitTest_itoa failed");
	JPT_RETURN_IF_LOG(!UnitTest_to_string(), "UnitTest_to_string failed");
}
