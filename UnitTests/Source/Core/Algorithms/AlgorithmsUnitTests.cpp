#include "Core/Building/JupiterPCH.h"
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
	char* itoaResult = jpt::itoa(12345);
	JPT_RETURN_FALSE_IF_LOG(strcmp(itoaResult, "12345") != 0, "");
	delete[] itoaResult;

	itoaResult = jpt::itoa(-12345);
	JPT_RETURN_FALSE_IF_LOG(strcmp(itoaResult, "-12345") != 0, "");
	delete[] itoaResult;

	itoaResult = jpt::itoa(0);
	JPT_RETURN_FALSE_IF_LOG(strcmp(itoaResult, "0") != 0, "");
	delete[] itoaResult;

	return true;
}

bool UnitTest_to_string()
{
	JPT_RETURN_FALSE_IF_LOG(jpt::to_string(0) != jpt::string("0"), "");
	JPT_RETURN_FALSE_IF_LOG(jpt::to_string(0) != "0", "");
	JPT_RETURN_FALSE_IF_LOG(jpt::to_string(12345) != jpt::string("12345"), "");
	JPT_RETURN_FALSE_IF_LOG(jpt::to_string(12345) != "12345", "");
	JPT_RETURN_FALSE_IF_LOG(jpt::to_string(-12345) != "-12345", "");
	JPT_RETURN_FALSE_IF_LOG(jpt::to_string(-12345) != "-12345", "");
	return true;
}

void RunAlgorithmsUnitTests()
{
	JPT_RETURN_IF_LOG(!UnitTest_strlen(), "UnitTest_strlen failed");
	JPT_RETURN_IF_LOG(!UnitTest_wcslen(), "UnitTest_wcslen failed");
	JPT_RETURN_IF_LOG(!UnitTest_itoa(), "UnitTest_itoa failed");
	JPT_RETURN_IF_LOG(!UnitTest_to_string(), "UnitTest_to_string failed");
}
