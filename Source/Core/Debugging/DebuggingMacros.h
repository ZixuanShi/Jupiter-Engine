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
