// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "MathUnitTests.h"

bool UnitTest_CommonMath()
{
	int32 num = 100;

	// Clamp
	jpt::ClampTo(num, 5, 10);
	JPT_RETURN_FALSE_IF_LOG(num != 10, "");
	num = jpt::GetClamped(5, 1, 10);
	JPT_RETURN_FALSE_IF_LOG(num != 5, "");

	// Abs
	JPT_RETURN_FALSE_IF_LOG(jpt::GetAbs(-100.05f) != 100.05f, "");
	JPT_RETURN_FALSE_IF_LOG(jpt::GetAbs(100.05f) != 100.05f, "");

	// Are values close
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreValuesClose(10, 10), "");
	JPT_RETURN_FALSE_IF_LOG(jpt::AreValuesClose(11, 10), "");
	JPT_RETURN_FALSE_IF_LOG(jpt::AreValuesClose(11.0f, 10.0f), "");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreValuesClose(11.0f, 11.0f), "");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreValuesClose(0.0000001, 0.0), "");

	return true;
}

void RunMathUnitTests()
{
	JPT_RETURN_IF_LOG(!UnitTest_CommonMath(), "UnitTest_CommonMath failed");
}