// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

/* If the condition has occured, log an messageand return a value
	@param condition:	The expression of the operation
	@param returnValue: The value to return if condition occured
	@param message:		The message to log if condition occured */
#define JPT_RETURN_VALUE_IF_LOG(condition, returnValue, message, ...)\
	if ((condition))\
	{\
		JPT_LOG(message, __VA_ARGS__);\
		return returnValue;\
	}

#define JPT_RETURN_VALUE_IF(condition, returnValue)\
	if ((condition))\
	{\
		return returnValue;\
	}

#define JPT_RETURN_FALSE_IF_LOG(condition, message, ...)\
	JPT_RETURN_VALUE_IF_LOG(condition, false, message, __VA_ARGS__);

#define RETURN_FALSE_IF(condition)\
	JPT_RETURN_VALUE_IF(condition, false);

#define JPT_RETURN_IF_LOG(condition, message, ...)\
	JPT_RETURN_VALUE_IF_LOG(condition, void(), message, __VA_ARGS__);

#define JPT_RETURN_IF(condition)\
	JPT_RETURN_VALUE_IF(condition, void());

/* Helper for cleanand delete.
    @param pPointer: Assumed to have Clean() function implemented */
#define JPT_CLEAN_THEN_DELETE(pPointer)\
	pPointer->Clean();\
	JPT_DELETE(pPointer);

/* __FILE__ but with L"XXX" wide string */
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)