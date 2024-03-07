// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#if IS_DEBUG || IS_DEVELOPMENT

namespace jpt
{
	/** Implementation when an assertion has failed
		@param line:	   Line of the file caused assertion fail
		@param file:       The file where assertion failed
		@param expression: Stringified assert condition.
		@param format:     Optional. Used to append additional debug info if there's any provided. Follow printf format
		@param ... :       Optional. Variadic arguments to append to assertion. */
	void OnAssertionFailed(int line, const char* file, const char* expression, const char* format, ...);
	void OnAssertionFailed(int line, const char* file, const char* expression);

	/** Assert with error message logging if Debug mode, do nothing if not Debugging
		@param expression: A boolean expression to assert true
		@param ...:	       Error messages if the expression went false

		Example:
		int a = 10;
		int b = 11;
		JPT_ASSERT(a == 9 && b == 10);
		JPT_ASSERT(a == 9 && b == 10, "expected 9 and 10");
		JPT_ASSERT(a == 9 && b == 10, "expected 9 and 10, received %d and %d", a, b);*/
#define JPT_ASSERT(expression, ...) \
		do\
		{\
			if (!(expression))\
			{\
				jpt::OnAssertionFailed(__LINE__, __FILE__, #expression, __VA_ARGS__);\
				__debugbreak();\
			}\
		} while(false)
}
#else
	#define JPT_ASSERT(expression, ...) 

#endif // IS_DEBUG || IS_DEVELOPMENT