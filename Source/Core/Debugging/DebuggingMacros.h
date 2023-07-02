// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#ifdef JPT_ENABLE_ASSERTS

	/** Assert with error message logging if Debug mode, do nothing if not Debugging
		@param expression: A boolean expression to assert true
		@param ...:	       Error messages if the expression went false 
		@example: 
		
		int a = 10;
		JPT_ASSERT(a == 9);	// output [Error] Assertion Failed: a == 9
		JPT_ASSERT(a == 9, "Expected 9"); // output [Error] Assertion Failed: a == 9, Expected 9 */
	#define JPT_ASSERT(expression, ...) \
		if (!(expression))\
		{\
			if (HAS_ARGS(#__VA_ARGS__))\
			{\
				JPT_ERROR("Assertion Failed: "#expression##", %s", __VA_ARGS__);\
			}\
			else\
			{\
				JPT_ERROR("Assertion Failed: "#expression);\
			}\
			__debugbreak();\
		}
#else
	#define JPT_ASSERT(expression, ...) 
#endif
