#pragma once

// If the condition has occured, log an message and return a value
// - condition: The expression of the operation
// - returnValue: The value to return if condition occured
// - message: The message to log if condition occured
#define RETURN_VALUE_IF_LOG(condition, returnValue, message, ...)\
	if ((condition))\
	{\
		JPT_LOG(message, __VA_ARGS__);\
		return returnValue;\
	}

#define RETURN_VALUE_IF(condition, returnValue)\
	if ((condition))\
	{\
		return returnValue;\
	}

#define RETURN_FALSE_IF_LOG(condition, message, ...)\
	RETURN_VALUE_IF_LOG(condition, false, message, __VA_ARGS__);

#define RETURN_FALSE_IF(condition)\
	RETURN_VALUE_IF(condition, false);

#define RETURN_IF_LOG(condition, message, ...)\
	RETURN_VALUE_IF_LOG(condition, void(), message, __VA_ARGS__);

#define RETURN_IF(condition)\
	RETURN_VALUE_IF(condition, void());

// Helper for clean and delete.
// - pPointer: Assumed to have Clean() function implemented
#define CLEAN_THEN_DELETE(pPointer)\
	pPointer->Clean();\
	JPT_DELETE(pPointer);
	