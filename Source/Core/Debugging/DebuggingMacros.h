#pragma once

// Assert with error message logging if Debug mode, do nothing if not Debugging
// - suceeded: A boolean expression to assert true
// - ...:	   Error messages if the succeeded went false
#ifdef JPT_ENABLE_ASSERTS
	#include <assert.h>

	#define JPT_ASSERT(succeeded, ...) \
		if (!(succeeded))\
		{\
			JPT_ERROR(__VA_ARGS__);\
		}\
		assert((succeeded));

	#define JPT_STATIC_ASSERT(succeeded, message) \
		static_assert(succeeded, message);
#else
	#define JPT_ASSERT(succeeded, ...) 
	#define JPT_STATIC_ASSERT(succeeded, message) 
#endif

// For Unit Testing
// Run a head function of section that contains multiple sub unit testings
// i.e. When testing Algorithms, add RUN_UNIT_TESTS(UnitTest_strlen); where UnitTest_strlen is a function contains unit tests for strlen
#define JPT_RUN_UNIT_TESTS(testsFunction)\
	if (!testsFunction())\
	{\
		JPT_ERROR("(%s) didn't pass Unit test", #testsFunction);\
	}

// Run a single sub unit test function and check result.
// i.e. RUN_UNIT_TEST(jpt::strlen("12345"), 5);
#define JPT_RUN_UNIT_TEST(outcome, expectedResult)\
	if (outcome != expectedResult)\
	{\
		JPT_ERROR("(%s) failed, expected (%s)", #outcome, #expectedResult);\
		return false;\
	}

// Check result. Run it if a function got passed in
// i.e.
// itoaResult = jpt::itoa(0);
// CHECK_RESULT(strcmp(itoaResult, "0") == 0);
// delete[] itoaResult;
#define JPT_CHECK_RESULT(result)\
	if (!(result))\
	{\
		JPT_ERROR("%s failed", #result);\
		return false;\
	}