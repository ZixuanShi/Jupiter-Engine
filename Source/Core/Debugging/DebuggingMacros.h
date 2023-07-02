// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#ifdef JPT_ENABLE_ASSERTS

	/** Assert with error message logging if Debug mode, do nothing if not Debugging
		@param expression: A boolean expression to assert true
		@param ...:	       Error messages if the expression went false */
	#define JPT_ASSERT(expression, ...) \
		if (!(expression))\
		{\
			JPT_ERROR("Assertion Failed: ", __VA_ARGS__);\
			__debugbreak();\
		}

#else
	#define JPT_ASSERT(expression, ...) 
#endif
